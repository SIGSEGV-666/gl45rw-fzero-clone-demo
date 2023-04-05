import bpy, bmesh
from collections import OrderedDict, deque
import itertools
from functools import reduce
import sys
from mathutils import Matrix, Vector, Euler
import json
import struct, math, random, string, os
from io import IOBase, BytesIO

DJDDF_VERSION_KEY = b"DDFVER"
DMD_MAGIC = b"DJMODEL\0"

DMDNT_EMPTY = b"EMPT"
DMDNT_MESH = b"MESH"
DMDUV_PERVERTEX = 0
DMDUV_PERINDEX = 1
DMD_MAX_TEXSLOTS_PER_MAT = 8

DMDBT_NONE = 0
DMDBT_ALPHA_BLEND = 1

DMDTB_MIX = 0
DMDTB_ADD = 1
DMDTB_MULTIPLY = 2

DMD_MAPTYPE_UVMAP = 0
DMD_MAPTYPE_REFLECTION = 1
DMD_MAPTYPE_VIEW = 2

DMD_FACECULL_NONE = 0
DMD_FACECULL_BACK = 1
DMD_FACECULL_FRONT = 2

DMD_TEXTYPE_IMAGE = 0
DMD_TEXTYPE_EQUIRECT = 1
DMD_TEXTYPE_CUBEMAP = 2

DMD_TEXSRC_IMAGE = 0
DMD_TEXSRC_NOISE = 1

DMD_PHYS_SHAPE_NONE = 0
DMD_PHYS_SHAPE_TRIMESH = 1

DMD_TEXFMT_RIM = 0
#DMD_TEXFMT_PNG = 1

DMD_FDT_I32 = 0
DMD_FDT_U32 = 1
DMD_FDT_F32 = 2

DMD_FDT_I64 = 3
DMD_FDT_U64 = 4
DMD_FDT_F64 = 5

DMD_FXDT_STRUCTFMTS = (
    'i',
    'I',
    'f',
    'q',
    'Q',
    'd'
)

def addAndGetIndex(lst, item):
    if item in lst:
        return lst.index(item)
    else:
        index = len(lst)
        lst.append(item)
        return index
def _writestruct(f, fmt, *data):
    return f.write(struct.pack(fmt, *data))

def _ddf_writepairs_v2(f, pairs, terminator=b"TERMKEY"):
    oldpos = f.tell()
    f.write(DJDDF_VERSION_KEY.ljust(8, b'\0'))
    _writestruct(f, "<I", 2)

    for key, value in pairs:
        f.write(key.ljust(8, b'\0'))
        _writestruct(f, "<I", len(value))
        f.write(value)
    
    f.write(terminator.ljust(8, b'\0'))

    return f.tell() - oldpos
def _write_bimg_as_rim(f, bimg):
    oldpos = f.tell()
    f.write(b"RAWIMG\0")
    w, h = bimg.size
    c = bimg.channels
    _writestruct(f, "<3I", w, h, c)
    pxv = bimg.pixels
    pxi = 0
    for y in range(h):
        for x in range(w):
            cvtpixel = bytearray(b"\0"*4)
            for d in range(c):
                cvtpixel[d] = min(max(int(255*pxv[pxi+d]), 0), 255)
            pxi += c
            f.write(cvtpixel[:c:])
                
    return f.tell() - oldpos
def export_dmd(operator, context, **k):
    _pack = struct.pack
    filepath = k['filepath']
    print(filepath)
    dmdstrings = []
    dmdnodes = []
    dmdmeshes = []
    dmdsubmdls = []
    dmdmaterials = []
    dmdtexdatas = []
    dmdteximages = []
    
    class _material(object):
        name = ""
        index = 0
        blmat = None

        base_diffuse_color = (1,1,1)
        base_diffuse_alpha = 1
        base_diffuse_intensity = 1

        shadeless = True

        base_specular_color = (1,1,1)
        base_specular_alpha = 1
        base_specular_intensity = 1
        base_specular_hardness = 0.1

        blendtype = DMDBT_NONE

        def __iter__(self):
            pairs = []
            for i in range(8):
                pairs.append((b"TEXSLOT", _pack("<IBI", i, 0, 0)))
            pairs.extend([
                (b"BDC", _pack("<3d", *self.base_diffuse_color)),
                (b"BSC", _pack("<3d", *self.base_specular_color)),
                (b"BDI", _pack("<d", self.base_diffuse_intensity)),
                (b"BSI", _pack("<d", self.base_specular_intensity)),
                (b"BDA", _pack("<d", self.base_diffuse_alpha)),
                (b"BSA", _pack("<d", self.base_specular_alpha)),
                (b"BSH", _pack("<d", self.base_specular_hardness)),
                (b"BLENDT", _pack("<B", self.blendtype)),
                (b"SHDLESS", _pack("<B", int(self.shadeless)))
            ])
            return iter(pairs)
        def __init__(self, _blmat):
            self.name = str(_blmat)
            self.blmat = _blmat
            self.index = len(dmdmaterials)
            dmdmaterials.append(self)
    class _meshdata(object):
        index = 0
        class _vertex(object):
            co = (0,0,0)
            color = (1,1,1)
            normal = (0,0,1)

            uv0 = (0,0)
            uv1 = (0,0)
            uv2 = (0,0)
            uv3 = (0,0)
            uv4 = (0,0)
            uv5 = (0,0)
            uv6 = (0,0)
            uv7 = (0,0)
        
        vertices = None
        nuvmaps = 0
        face_indices = None
        material = None
        _cvertindex = 0
        def __iter__(self):
            pairs = []

            coords = []
            colors = []
            normals = []
            faces = []
            uvmaps = []
            for v in self.vertices:
                for i in range(self.nuvmaps):
                    uv = (v.uv0, v.uv1, v.uv2, v.uv3, v.uv4, v.uv5, v.uv6, v.uv7)[i]
                    uvmaps.append(_pack("<2d", *uv))
                coords.append(_pack("<3d", *v.co))
                colors.append(_pack("<3d", *v.color))
                normals.append(_pack("<3d", *v.normal))
            for f in self.face_indices:
                faces.append(_pack("<3I", *f))
            pairs.extend([
                (b"UVMAPS", b"".join(uvmaps)),
                (b"VERTS", b"".join(coords)),
                (b"COLORS", b"".join(colors)),
                (b"NORMALS", b"".join(normals)),
                (b"FACES", b"".join(faces)),
                (b"MATINDEX", _pack("<I", self.material.index))
            ])
            return iter(pairs)
        def __init__(self):
            self.vertices = []
            self.face_indices = []
            self.index = len(dmdmeshes)
            dmdmeshes.append(self)
    class _node(object):
        name = ""
        parent = None
        children = None

        locpos = (0,0,0)
        locorn = ((1,0,0),(0,1,0),(0,0,1))
        locscale = (1,1,1)

        nodetype = DMDNT_EMPTY

        submodel = None

        index = 0
        nameindex = 0
        def __iter__(self):
            pairs = []
            if self.parent is not None:
                pairs.append(
                    (b"PNINDEX", _pack("<I", self.parent.index))
                )
            pairs.extend([
                (b"NINDEX", _pack("<I", self.nameindex)),
                (b"NTYPE", self.nodetype[0:4]),
                (b"LOCPOS", _pack("<3d", *self.locpos)),
                (b"LOCORN", b"".join((_pack("<3d", *r) for r in self.locorn))),
                (b"LOCSCL", _pack("<3d", *self.locscale))
            ])
            if (self.nodetype == DMDNT_MESH):
                pairs.append(
                    (b"SUBMDL", _pack("<I", self.submodel.index))
                )
            return iter(pairs)
        
        def __init__(self, _name, _parnode=None):
            self.name = str(_name)
            self.nameindex = addAndGetIndex(dmdstrings, self.name)
            self.parent = _parnode
            self.children = []
            self.index = len(dmdnodes)
            dmdnodes.append(self)
            if self.parent is not None:
                self.parent.children.append(self)
            
    class _submodel(object):
        meshes = None
        index = 0
        def __iter__(self):
            pairs = []
            meshlist = [_pack("<I", len(self.meshes))]
            for m in self.meshes:
                meshlist.append(_pack("<I", m.index))
            pairs.append((b"MESHES", b"".join(meshlist)))
            return iter(pairs)
        def __init__(self):
            self.meshes = []
            self.index = len(dmdsubmdls)
            dmdsubmdls.append(self)
    root_object = context.active_object
    objque = [dict(obj=root_object, parentnode=None, node=None)]
    while objque:
        oqe = objque.pop(0)
        is_root = oqe['obj'] is root_object
        oqe['node'] = objnode = _node(oqe['obj'].name, oqe['parentnode'])
        obj = oqe['obj']
        
    with open(filepath, "wb") as dmdf:
        dmdf.write(DMD_MAGIC)
        _writestruct(dmdf, "<I", 1)
        _ddf_writepairs_v2(dmdf, (
            (b"NSTRS", _pack("<I", len(dmdstrings))),
            (b"NNODES", _pack("<I", len(dmdnodes))),
            (b"NMESHES", _pack("<I", len(dmdmeshes))),
            (b"NSMDLS", _pack("<I", len(dmdsubmdls))),
            (b"NMATS", _pack("<I", len(dmdmaterials))),
            (b"NTEXDATS", _pack("<I", len(dmdtexdatas))),
            (b"NTEXIMGS", _pack("<I", len(dmdteximages))),
            (b"XDATSIZE", _pack("<I", 0))
        ))
        for s in dmdstrings:
            es = s.encode("ascii")
            _writestruct(dmdf, "<H", len(es))
            dmdf.write(es)
        
    return {'FINISHED'}