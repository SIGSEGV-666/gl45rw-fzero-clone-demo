#include "gl45rw/gl45rw_gles3_deferred_lights.hpp"
using namespace gl45rw;

const gfxapi _jstester_gfxapi =
#if (defined(GL45RW_WITH_OPENGL))
    gfxapi::opengl
#elif (defined(GL45RW_WITH_SW3D2))
    gfxapi::sw3d2
#elif (defined(GL45RW_WITH_SW3D3))
    gfxapi::sw3d3
#else
    gfxapi::none
#endif
;
class JoystickTester : public EngineInstance {
    using EngineInstance::EngineInstance;
    public:
        boolstate selleft_bs, selright_bs;
        int curjs = 0;
        BitmapFont* dispfont = nullptr;
        fscalar_t fontnpx = 15.0f;
        bool _mustclose = false;
        virtual void _onCreate(void* params) {
            this->R_SetVideoMode(videomode_t{
                .renderer=_jstester_gfxapi,
                .width=512,
                .height=386,
                .title=U"Joystick Tester"
            });

            this->dispfont = this->new_asset_rp<BitmapFont>();
            this->dispfont->load_default_font();

        }
        virtual void _onWindowCloseEvent(){
            this->_mustclose = true;
        }
        virtual bool update(const MainloopData& mld) {
            if (this->_mustclose || (this->E_GetInputValue("escape") > 0.5f)){return false;}

            if ((this->selleft_bs = (this->E_GetInputValue("left") > 0.5f)) == 2){
                this->curjs--;
                if (this->curjs < 0){this->curjs = GL45RW_MAX_GAMEPADS-1;}
            }
            if ((this->selright_bs = (this->E_GetInputValue("right") > 0.5f)) == 2){
                this->curjs++;
                if (this->curjs >= GL45RW_MAX_GAMEPADS){this->curjs = 0;}
            }

            auto& cgs = _gl45rw_common_gamepad_states[this->curjs];

            this->R_MakeCurrent();
            this->R_ClearBuffers(true,true);

            auto cvm = this->R_GetCurrentVideoMode();
            this->matrices.projection[-1] = glm::orthoRH_NO(-0.5f, 0.5f, -0.5f, 0.5f, -1.0f, 1.0f);
            this->matrices.model.loadIdentity();
            this->matrices.view.loadIdentity();

            std::stringstream ss;
            ss << "\n%cgUse the %cwLeft%cg & %cwRight%cg arrow keys\nto select a joystick.\n\n";
            ss << "%cwJoystick %cg" << (this->curjs+1) << " %cw/%cg " << GL45RW_MAX_GAMEPADS << "\n";
            ss << "%cwStatus: " << (cgs.connected ? "%cgCONNECTED" : "%crDISCONNECTED") << "\n";
            ss << "%cwName: %cg" << escape_raw_text(cgs.devname) << "\n";
            ss << "%cwNo. Buttons: %cg" << cgs.numbuttons << "\n";
            ss << "%cwNo. Axes : %cg" << cgs.numaxes << "\n";
            ss << "%cwActive Buttons:\n";
            for (int i = 0; i < cgs.numbuttons; i++){
                std::string btn_s = "gp" + std::to_string(this->curjs) + "b" + std::to_string(i);
                if (this->E_GetInputValue(btn_s) > 0.0f){ss << " %cg" << i;}
            }
            ss << "\n";

            ss << "%cwAxis Values:\n";
            for (int i = 0; i < cgs.numaxes; i++){
                std::string axis_s = "gp" + std::to_string(this->curjs) + "a" + std::to_string(i);
                fscalar_t value = this->E_GetInputValue(axis_s + "+") - this->E_GetInputValue(axis_s + "-");
                char value_str[1024] = {};
                sprintf(value_str, "%+0.4f", value);
                ss << " %cwAxis " << i << " = %cy" << value_str << '\n';
            }


            trs_t text_trs; text_trs.pos = fvec3_t{-0.45f,0.45f,0};
            text_trs.scale = fvec3_t{this->fontnpx/cvm.width, this->fontnpx/cvm.height, 1.0f};
            this->matrices.model[-1] = fmat44_t(text_trs);
            this->R_DrawBitmapText(this->dispfont[0], djutil::ezstr::utf8_to_utf32(ss.str()));

            this->R_SwapBuffers();

            return true;
        }
};


int main() {
    global_startup();
    auto* i1 = new_engine_instance<JoystickTester>(nullptr);
    global_mainloop();
    global_shutdown();
    return 0;
}
