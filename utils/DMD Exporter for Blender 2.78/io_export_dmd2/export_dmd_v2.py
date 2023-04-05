import bpy, bmesh
from collections import OrderedDict, deque
import itertools
from functools import reduce
import sys
from mathutils import Matrix, Vector, Euler
import json
import struct, math, random, string, os, operator
from io import IOBase, BytesIO
import bgl
DJDDF_VERSION_KEY = b"DDFVER"
DMD_MAGIC = b"DJMODEL\0"

DMDNT_EMPTY = b"EMPT"
DMDNT_MESH = b"MESH"
DMDUV_PERVERTEX = 0
DMDUV_PERINDEX = 1
DMD_MAX_TEXSLOTS_PER_MAT = 8

DMDBT_NONE = 0
DMDBT_ALPHA_BLEND = 1
DMDBT_ADD = 2

DMDTB_MIX = 0
DMDTB_ADD = 1
DMDTB_MULTIPLY = 2

BTMAP = {"MIX": DMDTB_MIX, "ADD": DMDTB_ADD, "MULTIPLY": DMDTB_MULTIPLY}

DMD_BUMPSPACE_OBJECT = 0
DMD_BUMPSPACE_TANGENT = 1

GLCHANMAP = (None,
    bgl.GL_LUMINANCE,
    bgl.GL_LUMINANCE_ALPHA,
    bgl.GL_RGB,
    bgl.GL_RGBA
)

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
DMD_TEXSRC_EQUIRECT = 2

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



def axis_angle_between(v1, v2):
    return v1.cross(v2).normalized(), math.acos(v1.dot(v2))


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
    #pxv = tuple(bimg.pixels)
    #pxb = bytearray(b"\0"*(w*h*c))
    #for pxi, pxc in enumerate(pxv):
    #    pxb[pxi] = min(max(int(255*pxc),0),255)
    pxb = bgl.Buffer(bgl.GL_BYTE, [w*h*c])
    bgl.glBindTexture(bgl.GL_TEXTURE_2D, bimg.bindcode[0])
    bgl.glGetTexImage(bgl.GL_TEXTURE_2D, 0, GLCHANMAP[c], bgl.GL_UNSIGNED_BYTE, pxb)
    bgl.glBindTexture(bgl.GL_TEXTURE_2D, 0)
    f.write(bytearray(pxb.to_list()))
    return f.tell() - oldpos

def guess_fxdt(elems):
    e0 = elems[0]
    if isinstance(e0, int):
        return DMD_FDT_I64
    elif isinstance(e0, float):
        return DMD_FDT_F64
    else:
        raise ValueError("couldn't determine dtype!")
def parse_dmdfx(src):
    return json.loads(src)

class DMD_FXPointer(object):
    fxcat = 0
    fxnum = 0
    args = None
    def __init__(self, *a, **k):
        self.args = dict()
    @classmethod
    def from_data(cls, e, slist):
        self = cls()
        #print(en)
        #e = _getfxent(en)

        self.fxcat = int(e['fxcat'])
        self.fxnum = int(e['fxnum'])
        print("fxcat = " + str(self.fxcat))
        print("fxnum = " + str(self.fxnum))
        for an, av in e.get("args", {}).items():
            if not hasattr(av, "__iter__"):
                av = (av,)
            sidx = addAndGetIndex(slist, an)
            self.args[an] = dict(nameindex=sidx, datatype=guess_fxdt(av), elements=av)
        return self
    def write(self, f):
        _writestruct(f, "<II", self.fxcat, self.fxnum)
        datsz_pos = f.tell() #record the position of fxdatsz to write it later.
        f.write(b"\0"*4) #write 4 zeros for now
        startpos = f.tell() #record the start of the data.
        pairs = []
        if self.args is not None:
            for key, value in self.args.items():
                bio = BytesIO()
                _writestruct(bio, "<I", 0) #write version number
                _writestruct(bio, "<III", value['nameindex'], value['datatype'], len(value['elements']))
                #print("dmdfx key = %r" % (key,))
                for i, e in enumerate(value['elements']):
                    fmt = "<"+(DMD_FXDT_STRUCTFMTS[value['datatype']])
                    #print("dmdfx fmt = %r" % (fmt,))
                    #print("dmdfx value[%i] = %r" % (i, e))
                    _writestruct(bio, fmt, e)
                pairs.append((b"FXARG", bio.getvalue()))
        _ddf_writepairs_v2(f, pairs)
        #endpos = f.tell()
        #f.seek(datsz_pos)
        #_writestruct(f, "<I", endpos-startpos) #write the actual data size to the datasize header entry
        #.seek(endpos) #return back to the last pos
    def tobytes(self):
        bio = BytesIO()
        self.write(bio)
        return bio.getvalue()
def export_dmd_v2(operator, context, **k):
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
    dmdmatmap = {}
    fxdatas = {}
    for t in bpy.data.texts:
        n = t.name
        if (n.endswith(".dmdfx")):
            value = fxdatas[n.partition(".")[0]] = json.loads("{"+(t.as_string())+"}")
            print(n, value)
    def _getfxent(en):
        before, sep, after = en.partition(".")
        return fxdatas[before]['entries'][after]



    #def _convertfxent(en, bdata):
    #    e = _getfxent(en)
    #    df = DMD_FXPointer(bdata.dmdfile, bdata)
    #    df.fxcat = e['fxcat']
    #    df.fxnum = e['fxnum']
    #    for an, av in e.get("args", {}).items():
    #        if not hasattr(av, "__iter__"):
    #            av = [av]
    #        addAndGetIndex(d.stringlist, an)
    #        df.args[an] = dict(datatype=guess_fxdt(av), elements=av)
    #    return df
    bimg2teximg = {}
    class _teximage(object):
        index = 0
        bimage = None
        def write_header(self, f):
            return _writestruct(f, "<III", 0, 1, 1)
        def write_subimages(self, f):
            pairs = [
                (b"ORIGIN", _pack("<B", 2))
            ]
            return _ddf_writepairs_v2(f, pairs) + _write_bimg_as_rim(f, self.bimage)
        def __init__(self, _bimage):
            self.bimage = _bimage
            self.index = len(dmdteximages)
            dmdteximages.append(self)
            bimg2teximg[self.bimage.name] = self
            print("new teximage: " + repr(self.bimage.name))
    class _texdata(object):
        index = 0
        btslot = None
        textype = DMD_TEXTYPE_IMAGE
        texsrc = DMD_TEXSRC_IMAGE
        maptype = DMD_MAPTYPE_UVMAP
        uvmapno = 0
        dimage = None

        class _texaffect:
            use = False
            amount = 1
            def asbytes(self):
                return struct.pack("<Bd", int(self.use), float(self.amount))
            def __init__(self, _use=False, _amount=0):
                self.use = bool(_use)
                self.amount = float(_amount)

        affects_diffuse_color = None
        affects_diffuse_alpha = None
        affects_diffuse_intensity = None

        affects_specular_color = None
        affects_specular_alpha = None
        affects_specular_intensity = None
        affects_specular_hardness = None

        affects_ambient_color = None

        affects_texture_offset = None

        affects_normal_vector = None

        affects_emit_amount = None

        blendtype = DMDTB_MIX
        bumpspace = DMD_BUMPSPACE_OBJECT
        texfx = None

        use_rgb2intensity = False
        rgb2intensity_color = (1,1,1)

        def __iter__(self):
            pairs = [
                (b"TEXIMG", _pack("<I", (self.dimage.index if self.dimage is not None else 0))),
                (b"BLENDT", _pack("<B", self.blendtype)),
                (b"UVMAP", _pack("<I", self.uvmapno)),
                (b"MAPTYPE", _pack("<H", self.maptype)),
                (b"TEXSRC", _pack("<I", self.texsrc)),
                (b"BMSPACE", _pack("<B", self.bumpspace))
            ]
            if (self.use_rgb2intensity):
                pairs.append( (b"RGB2INT", _pack("<3d", *self.rgb2intensity_color)) )
            if (self.texfx is not None):
                pairs.append( (b"TEXFX", self.texfx.tobytes()) )
            for key, attrn in (
                (b"ADC", "diffuse_color"),
                (b"ADA", "diffuse_alpha"),
                (b"ADI", "diffuse_intensity"),

                (b"ASC", "specular_color"),
                (b"ASI", "specular_intensity"),
                (b"ASH", "specular_hardness"),
                (b"ASA", "specular_alpha"),
                (b"ATO", "texture_offset"),
                (b"ANV", "normal_vector"),
                (b"AEA", "emit_amount"),
                (b"AAC", "ambient_color")
            ):
                pairs.append((key, getattr(self, "affects_"+attrn).asbytes()))
            return iter(pairs)
        def __init__(self, _btslot, _btmesh):
            self.index = len(dmdtexdatas)
            dmdtexdatas.append(self)

            self.btslot = _btslot

            self.affects_diffuse_color = self._texaffect(_btslot.use_map_color_diffuse, _btslot.diffuse_color_factor)
            self.affects_diffuse_alpha = self._texaffect(_btslot.use_map_alpha, _btslot.alpha_factor)
            self.affects_diffuse_intensity = self._texaffect(_btslot.use_map_diffuse, _btslot.diffuse_factor)

            self.affects_specular_color = self._texaffect(_btslot.use_map_color_spec, _btslot.specular_color_factor)
            self.affects_specular_intensity = self._texaffect(_btslot.use_map_specular, _btslot.specular_factor)
            self.affects_specular_hardness = self._texaffect(_btslot.use_map_hardness, _btslot.hardness_factor)
            #print(_btslot.texture_coords)
            #self.maptype = dict(ss_factor)
            self.affects_specular_alpha = self.affects_diffuse_alpha

            self.affects_texture_offset = self._texaffect(_btslot.use_map_warp, _btslot.warp_factor)
            self.affects_normal_vector = self._texaffect(_btslot.use_map_normal, _btslot.normal_factor)

            self.affects_emit_amount = self._texaffect(_btslot.use_map_emit, _btslot.emit_factor)
            self.affects_ambient_color = self._texaffect(_btslot.use_map_ambient, _btslot.ambient_factor)

            self.blendtype = BTMAP.get(str(_btslot.blend_type), DMDTB_MIX)

            self.use_rgb2intensity = _btslot.use_rgb_to_intensity
            self.rgb2intensity_color = tuple(_btslot.color)[:3]

            if (_btslot.bump_objectspace == "BUMP_TEXTURESPACE"):
                self.bumpspace = DMD_BUMPSPACE_TANGENT
            else:
                self.bumpspace = DMD_BUMPSPACE_OBJECT
            self.maptype = dict(
                UV=0,
                REFLECTION=1,
                ORCO=2
            ).get(str(_btslot.texture_coords), 0)

            tex = _btslot.texture

            if "dmdfx" in tex:
                self.texfx = DMD_FXPointer.from_data(_getfxent(tex['dmdfx']), dmdstrings)
            else:
                self.texfx = None

            bimage = getattr(tex, "image", None)
            #is_imagetex = isinstance(tex, bpy.types.ImageTexture)
            is_imagetex = tex.type == "IMAGE"
            is_envtex = tex.type == "ENVIRONMENT_MAP"
            #is_envtex = isinstance(tex, bpy.types.EnvironmentMapTexture)
            #self.textype = DMD_TEXTYPE_IMAGE
            if (bimage is not None) and is_imagetex:
                self.texsrc = DMD_TEXSRC_IMAGE
                #print(_btslot.mapping)
                if str(_btslot.mapping) == "SPHERE":
                    self.texsrc = DMD_TEXSRC_EQUIRECT
                _uvnames = tuple(lay.name for lay in _btmesh.uv_layers)
                _default_uvmapno = (_uvnames.index(_btslot.uv_layer) if _btslot.uv_layer in _uvnames else 0)
                self.uvmapno = int(tex.get("dmd_uvmap_index", _default_uvmapno))
                print("UVMAP=" + repr(_btslot.uv_layer) + ", uvmapno=" + str(self.uvmapno))
                self.dimage = (_teximage(bimage) if not bimage.name in bimg2teximg else bimg2teximg[bimage.name])

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

        base_emit_amount = 0.0

        base_ambient_color = (0,0,0)

        blendtype = DMDBT_NONE
        facecull = DMD_FACECULL_BACK
        matfx = None
        class _texslot(object):
            enabled = False,
            texdata = None

            def __init__(self, _enabled=False, _texdata=None):
                self.enabled = bool(_enabled)
                self.texdata = _texdata
        texture_slots = None
        def __hash__(self):
            return hash(self.name)

        def __iter__(self):
            pairs = []
            for i, ts in enumerate(self.texture_slots):
                pairs.append((b"TEXSLOT", _pack("<IBI", i, ts.enabled, ts.texdata.index)))
            pairs.extend([
                (b"BDC", _pack("<3d", *self.base_diffuse_color)),
                (b"BSC", _pack("<3d", *self.base_specular_color)),
                (b"BAC", _pack("<3d", *self.base_ambient_color)),
                (b"BDI", _pack("<d", self.base_diffuse_intensity)),
                (b"BSI", _pack("<d", self.base_specular_intensity)),
                (b"BDA", _pack("<d", self.base_diffuse_alpha)),
                (b"BSA", _pack("<d", self.base_specular_alpha)),
                (b"BSH", _pack("<d", self.base_specular_hardness)),
                (b"BEA", _pack("<d", self.base_emit_amount)),
                (b"BLENDT", _pack("<B", self.blendtype)),
                (b"SHDLESS", _pack("<B", int(self.shadeless))),
                (b"FACECULL", _pack("<B", self.facecull))
            ])
            if (self.matfx is not None):
                pairs.append( (b"MATFX", self.matfx.tobytes()) )
            return iter(pairs)
        def __init__(self, _blmat, _btmesh):
            self.name = str(_blmat.name)
            print("New material: " + repr(self.name))
            self.blmat = _blmat
            self.index = len(dmdmaterials)
            dmdmaterials.append(self)
            dmdmatmap[self.name] = self

            self.base_diffuse_color = tuple(_blmat.diffuse_color)[0:3]
            self.base_specular_color = tuple(_blmat.specular_color)[0:3]

            self.base_diffuse_alpha = self.base_specular_alpha = _blmat.alpha
            self.base_diffuse_intensity = _blmat.diffuse_intensity
            self.base_specular_intensity = _blmat.specular_intensity
            self.base_specular_hardness = _blmat.specular_hardness/511.0
            self.base_emit_amount = _blmat.emit
            self.facecull = (DMD_FACECULL_NONE, DMD_FACECULL_BACK)[int(_blmat.game_settings.use_backface_culling)]
            #self.texture_slots = tuple(self._texslot() for _i in range(8))
            self.texture_slots = []
            self.shadeless = _blmat.use_shadeless
            if (_blmat.use_transparency):
                self.blendtype = {
                    "OPAQUE": DMDBT_NONE,
                    "ADD": DMDBT_ADD,
                    "ALPHA": DMDBT_ALPHA_BLEND,
                    "ALPHA_SORT": DMDBT_ALPHA_BLEND,
                    "ALPHA_ANTIALIASING": DMDBT_ALPHA_BLEND
                }.get(_blmat.game_settings.alpha_blend, DMDBT_NONE)
            texlist = tuple(filter(None, _blmat.texture_slots))[0:8]
            print(" num texslots = " + str(len(texlist)))
            for btsi, btslot in enumerate(texlist):
                self.texture_slots.append(self._texslot(_blmat.use_textures[btsi], _texdata(btslot, _btmesh)))
    class _meshdata(object):
        index = 0
        class _vertex(object):
            co = (0,0,0)
            color = (1,1,1)

            tangent = (1,0,0)
            binormal = (0,1,0)
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
            tangents = []
            binormals = []
            normals = []
            faces = []
            uvmaps = [[] for i in range(self.nuvmaps)]
            for v in self.vertices:
                for i in range(self.nuvmaps):
                    uv = (v.uv0, v.uv1, v.uv2, v.uv3, v.uv4, v.uv5, v.uv6, v.uv7)[i]
                    #uvmaps.append(_pack("<2d", *uv))
                    uvmaps[i].append(_pack("<2d", *uv))
                coords.append(_pack("<3d", *v.co))
                colors.append(_pack("<3d", *v.color))
                tangents.append(_pack("<3d", *v.tangent))
                binormals.append(_pack("<3d", *v.binormal))
                normals.append(_pack("<3d", *v.normal))
            for f in self.face_indices:
                faces.append(_pack("<3I", *f))
            pairs.extend([
                (b"UVMAPS", b"".join((b"".join(uvm) for uvm in uvmaps))),
                (b"VERTS", b"".join(coords)),
                (b"COLORS", b"".join(colors)),
                (b"TANGENTS", b"".join(tangents)),
                (b"BINORMS", b"".join(binormals)),
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

        physshape = DMD_PHYS_SHAPE_NONE
        collmask = 1
        visible = True
        nodefx = None
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
                (b"LOCSCL", _pack("<3d", *self.locscale)),
                (b"PSHAPE", _pack("<H", self.physshape)),
                (b"COLLMASK", _pack("<I", self.collmask)),
                (b"VISIBLE", _pack("<B", int(self.visible)))
            ])
            if (self.nodetype == DMDNT_MESH):
                pairs.append(
                    (b"SUBMDL", _pack("<I", self.submodel.index))
                )
            if (self.nodefx is not None):
                pairs.append( (b"NODEFX", self.nodefx.tobytes()) )
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
    objque = [dict(level=0, obj=root_object, parentnode=None, node=None)]
    while objque:
        oqe = objque.pop(0)
        is_root = oqe['obj'] is root_object
        oqe['node'] = objnode = _node(oqe['obj'].name, oqe['parentnode'])
        obj = oqe['obj']

        if is_root:
            basemat = Matrix.Identity(4)
            pscale = Vector((1,1,1))
        else:
            basemat = Matrix(obj.matrix_parent_inverse)
            pscale = obj.parent.scale
        objnode.locpos = tuple(basemat*Vector(tuple(obj.location[0:3])+(1.0,)))[0:3]
        objnode.locorn = tuple(map(tuple, (basemat.to_3x3()*(obj.rotation_euler.to_matrix()))[0:3]))
        objnode.locscale = tuple(x/y for x, y in zip(obj.scale, pscale))
        objnode.visible = not obj.hide_render
        objnode.physshape = DMD_PHYS_SHAPE_NONE
        objnode.collmask = 0
        if (obj.game.physics_type != "NO_COLLISION"):
            objnode.physshape = DMD_PHYS_SHAPE_TRIMESH
            for i, v in enumerate(obj.game.collision_group):
                if v:
                    objnode.collmask |= 2**i
        for cobj in obj.children:
            objque.append(dict(level=oqe['level']+1, obj=cobj, parentnode=objnode, node=None))

        if obj.data:
            objnode.nodetype = DMDNT_MESH
            bm = bmesh.new()
            try:
                #obj.data.calc_tangents()
                bm.from_mesh(obj.data)
                bm.faces.ensure_lookup_table()
                uv_layers = bm.loops.layers.uv
                if not bm.loops.layers.color:
                    bm.loops.layers.color.new('Col')
                color_layer = bm.loops.layers.color[0]
                dmat2dmesh = {}
                for fi, face in enumerate(bm.faces):
                    fmat = None
                    try:
                        fmat = obj.material_slots[face.material_index].material
                    except IndexError:
                        pass
                    if fmat is None:
                        continue
                    if fmat.name not in dmdmatmap:
                        dmat = _material(fmat, obj.data)
                    else:
                        dmat = dmdmatmap[fmat.name]

                    if dmat.name not in dmat2dmesh:
                        dmat2dmesh[dmat.name] = dmesh = _meshdata()
                        dmesh.nuvmaps = min(len(uv_layers), 8)
                        dmesh.material = dmat
                    dmesh = dmat2dmesh[dmat.name]

                    dmesh.face_indices.append([])
                    dface = dmesh.face_indices[-1]
                    uvnames = tuple("uv"+str(_i) for _i in range(8))
                    if len(face.verts) != 3:
                        continue
                    #tangent = face.calc_tangent_vert_diagonal();
                    face_normal = face.normal
                    #face_tangent = face.calc_tangent_vert_diagonal()
                    #face_binormal = face_normal.cross(face_tangent).normalized()

                    for vn, (vert, loop) in enumerate(zip(face.verts, face.loops)):
                        dvertex = _meshdata._vertex()
                        dvertex.co = tuple(vert.co)
                        dvertex.color = tuple(loop[color_layer])[0:3]
                        dvertex.normal = tuple(vert.normal)
                        tangent = loop.calc_tangent().normalized()
                        #print(vn, tangent)
                        #dvertex.tangent = tuple(tangent)
                        #dvertex.binormal = tuple(-(loop.calc_normal().cross(tangent).normalized()))

                        #vert_normal = vert.normal
                        #traxis, trangle = axis_angle_between(face_normal, vert_normal)
                        #trmat = Matrix.Rotation(trangle, 3, traxis)
                        #dvertex.tangent = tuple((trmat * face_tangent).normalized())
                        #dvertex.binormal = tuple((trmat * face_binormal).normalized())
                        for uvmn, uv_lay in enumerate(uv_layers.values()):
                            if (uvmn >= 8): break;
                            setattr(dvertex, uvnames[uvmn], tuple(loop[uv_lay].uv)[0:2])
                        dface.append(dmesh._cvertindex)
                        dmesh._cvertindex += 1
                        dmesh.vertices.append(dvertex)
                dsubmdl = _submodel()
                dsubmdl.meshes = list(dmat2dmesh.values())
                objnode.submodel = dsubmdl
            finally:
                bm.free()
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
        for nodeindex, node in enumerate(dmdnodes):
            _writestruct(dmdf, "<I", 0)
            _ddf_writepairs_v2(dmdf, node)
        for dmeshindex, dmesh in enumerate(dmdmeshes):
            _writestruct(dmdf, "<IIIBI", 0, len(dmesh.vertices), dmesh.nuvmaps, DMDUV_PERVERTEX, len(dmesh.face_indices))
            _ddf_writepairs_v2(dmdf, dmesh)
        for dsubmdlindex, dsubmdl in enumerate(dmdsubmdls):
            _writestruct(dmdf, "<I", 0)
            _ddf_writepairs_v2(dmdf, dsubmdl)
        for dmatindex, dmat in enumerate(dmdmaterials):
            _writestruct(dmdf, "<I", 0)
            _ddf_writepairs_v2(dmdf, dmat)
        #texture data
        for dtexdatindex, dtexdat in enumerate(dmdtexdatas):
            _writestruct(dmdf, "<I", 0)
            _ddf_writepairs_v2(dmdf, tuple(dtexdat))
        #texture images
        for dteximgindex, dteximg in enumerate(dmdteximages):
            dteximg.write_header(dmdf)
            dteximg.write_subimages(dmdf)
        #extra data
    return {'FINISHED'}
