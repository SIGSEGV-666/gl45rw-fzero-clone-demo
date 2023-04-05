bl_info = {
    "name": "DJEngine DMD format (ver. 2)",
    "author": "D.J. Paul",
    "blender": (2, 74, 0),
    "location": "File > Import-Export",
    "description": "Export DMD, meshes, uvs, materials, textures, ",
    "warning": "",
    "category": "Import-Export"
}
import bpy, bpy_extras
from bpy.props import (
        BoolProperty,
        EnumProperty,
        FloatProperty,
        StringProperty,
        )
from bpy_extras.io_utils import (
        ImportHelper,
        ExportHelper,
        orientation_helper_factory,
        axis_conversion,
        )
class ExportDMD_V2(bpy.types.Operator, ExportHelper):
    """Export to DMD file format (.dmd)"""
    bl_idname = "export_object.djengine_dmd_v2"
    bl_label = 'Export DMD (addon version 2)'

    filename_ext = ".dmd"
    filter_glob = StringProperty(
            default="*.dmd",
            options={'HIDDEN'},
            )

    def execute(self, context):
        from . import export_dmd_v2

        #keywords = self.as_keywords(ignore=("axis_forward",
        #                                    "axis_up",
        #                                    "filter_glob",
        #                                    "check_existing",
        #                                    ))
        #global_matrix = axis_conversion(to_forward=self.axis_forward,
        #                                to_up=self.axis_up,
        #                                ).to_4x4()
        #keywords["global_matrix"] = global_matrix
        keywords = self.as_keywords()
        return export_dmd_v2.export_dmd_v2(self, context, **keywords)


# Add to a menu
def menu_func_export(self, context):
    self.layout.operator(ExportDMD_V2.bl_idname, text="DJModel (.dmd) (V2)")

def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()
