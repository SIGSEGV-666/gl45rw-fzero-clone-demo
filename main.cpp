#include "./gl45rw/gl45rw_gles3_deferred_lights.hpp"
using namespace gl45rw;

using entid_t = Scene::entid_t;
using entity = Scene::entity;

using EntityClass = Scene::EntityClass;

#define SPEED_SCALE 0.25f

fvec3_t fvec3_from_string(const std::string& lit) {
    fvec3_t v;
    std::stringstream ss; ss.str(lit);
    ss >> std::skipws >> v.x >> v.y >> v.z;
    return v;
}

fmat33_t orn_from_eul(const fvec3_t& eul) {
    return glm::eulerAngleXYZ(glm::radians(eul.x), glm::radians(eul.y), glm::radians(eul.z));
}
fmat33_t orn_from_eul(const std::string& euls) {
    fscalar_t ex = 0, ey = 0, ez = 0;
    std::stringstream ss; ss.str(euls);
    ss >> std::skipws >> ex >> ey >> ez;
    return glm::eulerAngleXYZ(glm::radians(ex), glm::radians(ey), glm::radians(ez));
}

class SpaceShooterMain : public EngineInstance {
    using EngineInstance::EngineInstance;
    public:
        bindcategory_t* default_bindcat = nullptr;
        Scene* main_scene = nullptr;
        entid_t player_eid = 0;
        bool _mustclose = false;
        SceneGraphModel
            *player_mdl = nullptr,
            *sky_mdl = nullptr
        ;

        class EC_light : public Scene::EntityClass {
            using Scene::EntityClass::EntityClass;
            public:
                struct params_t {
                    fvec3_t pos = {0,0,0};
                    fmat33_t orn = fmat33_t(1);
                    gl45rw::lightprops_t lprops = {};
                };

                virtual std::string classname() {return "light";}
            private:
                static void _light_render(Scene::entity& self) {
                    //auto* cls = (EC_testlight*)(self.scene->entity_classes[self.classname]);
                    auto* lsrc = (LightSource*)(self.userdata);
                    lsrc->addlight(LightSource::addparams_t{.pos=self.trs.pos, .orn=self.trs.orn});
                }
                static void _light_destroy(Scene::entity& self) {
                    auto* lsrc = (LightSource*)(self.userdata);
                    self.scene->instance->del_asset((uid_t)*lsrc);
                    self.userdata = nullptr;
                }
            protected:
                virtual Scene::entity* prtspawn(void* args){
                    auto& params = *(params_t*)args;
                    auto* e = this->scene->spawn();
                    e->classname = this->classname();
                    e->render = _light_render;
                    e->on_delete = _light_destroy;
                    e->trs.pos = params.pos;
                    e->trs.orn = params.orn;
                    auto* lsrc = this->scene->instance->new_asset_rp<LightSource>(params.lprops);
                    e->userdata = lsrc;
                    return e;
                }
                virtual Scene::entity* prtspawn_da(const dictionary<std::string, std::string>& dargs) {
                    params_t p;
                    p.pos = fvec3_from_string(dargs.getd("pos", "0 0 0"));
                    p.orn = fmat33_t(1);

                    if (dargs.count("orn_eul") != 0){
                        const fvec3_t _angles = glm::radians(fvec3_from_string(dargs.getd("orn_eul", "0 0 0")));
                        p.orn = glm::eulerAngleXYZ(_angles.x, _angles.y, _angles.z);
                    }

                    p.lprops.diffuse.enabled = std::stoi(dargs.getd("diffuse.enabled", "1"));
                    p.lprops.diffuse.color = fvec3_from_string(dargs.getd("diffuse.color", "1 1 1"));
                    p.lprops.diffuse.factor = std::stof(dargs.getd("diffuse.factor", "1"));

                    p.lprops.specular.enabled = std::stoi(dargs.getd("specular.enabled", "1"));
                    p.lprops.specular.color = fvec3_from_string(dargs.getd("specular.color", "1 1 1"));
                    p.lprops.specular.factor = std::stof(dargs.getd("specular.factor", "1"));

                    p.lprops.energy = std::stof(dargs.getd("energy", "10"));
                    p.lprops.radius = std::stof(dargs.getd("radius", "25"));

                    p.lprops.cone_angle = std::stof(dargs.getd("cone_angle", "90"));

                    std::string fallofftype = dargs.getd("falloff", "linear");
                    if (fallofftype == "constant"){p.lprops.falloff = gl45rw::lightfallofftype::constant;}
                    else if (fallofftype == "square"){p.lprops.falloff = gl45rw::lightfallofftype::square;}
                    else {p.lprops.falloff = gl45rw::lightfallofftype::linear;}

                    std::string lighttype = dargs.getd("type", "point");

                    if (lighttype == "spot"){
                        p.lprops.type = gl45rw::lightsrctype::spot;
                    }
                    else if (lighttype == "hemi"){
                        p.lprops.type = gl45rw::lightsrctype::hemi;
                    }
                    else {p.lprops.type = gl45rw::lightsrctype::point;}

                    auto* e = this->prtspawn(&p);
                    this->scene->set_tag(e, dargs.getd("tagname", ""));
                    return e;
                }
        };

        class EC_mapmodel : public Scene::EntityClass {
            using Scene::EntityClass::EntityClass;
            struct eparams_t {
                std::string u8path;
                bool force_nearest = false;
                fvec3_t pos = {0,0,0}, scale = {1,1,1};
                fmat33_t orn = fmat33_t(1);
            };
            struct mapmodel_userdata {
                SceneGraphModel* sgmdl = nullptr;
            };
            //dictionary<entid_t, mapmodel_userdata> _userdatas = {};
            private:
                static void _mapmodel_logic(Scene::entity& self, const gl45rw::MainloopData& mld) {
                    //std::cout << "mapmodel " << self.eid << " updated!\n";
                    ;
                }
                static void _mapmodel_render(Scene::entity& self){
                    auto* ud = (mapmodel_userdata*)self.userdata;
                    //self.scene->worldmdlque.push_back(Scene::mdlqueent_t{.mdl=ud->sgmdl, .eargs={.base_trs=self.trs}});
                    SceneGraphModel::enque_args eargs = {.base_trs=self.trs, .rgrpmask=Scene::SCENE_WORLDMODEL};
                    ud->sgmdl->enque(eargs);
                }
                static void _mapmodel_ondelete(Scene::entity& self) {
                    std::cout << "EC_mapmodel::_mapmodel_ondelete(" << self.eid << ");\n";
                    auto* ud = (mapmodel_userdata*)self.userdata;
                    self.scene->instance->del_asset(uid_t(*ud->sgmdl));
                    delete ud; self.userdata = nullptr;
                }
                static void _mapmodel_physics_upload(Scene::entity& self, const gl45rw::MainloopData& mld) {
                    auto* ud = (mapmodel_userdata*)self.userdata;
                    gl45rw::SceneGraphModel::enque_args ea = {.base_trs=self.trs, .enque_renderobjs=false, .enque_physobjs=true};
                    ud->sgmdl->enque(ea);
                }
            public:
                virtual std::string classname() {return "mapmodel";}
            protected:
                virtual Scene::entity* prtspawn(void* args) {
                    auto& params = *(eparams_t*)args;
                    auto* e = this->scene->spawn();
                    auto* ud = new mapmodel_userdata;
                    e->classname = this->classname();
                    e->logic = _mapmodel_logic;
                    e->render = _mapmodel_render;
                    e->on_delete = _mapmodel_ondelete;
                    e->physics_upload = _mapmodel_physics_upload;
                    ud->sgmdl = this->scene->instance->new_asset_rp<SceneGraphModel>();
                    ud->sgmdl->merge_model(djutil::ezstr::utf8_to_utf32(params.u8path));
                    if (params.force_nearest){
                        for (auto& aid : ud->sgmdl->owns_assets) {
                            auto atype = this->scene->instance->get_asset_type(aid);
                            if (atype == gl45rw::assettype::texture) {
                                auto* tex = this->scene->instance->get_asset<Texture2D>(aid);
                                tex->_minfilter = tex->_magfilter = gl45rw::texfilter::nearest;
                                tex->update_texparams();
                            }
                        }
                    }
                    e->trs.pos = params.pos;
                    e->trs.scale = params.scale;
                    e->trs.orn = params.orn;
                    e->userdata = ud;
                    return e;
                }
                virtual Scene::entity* prtspawn_da(const dictionary<std::string, std::string>& dargs) {
                    eparams_t p; p.u8path = dargs.getd("path", "/models/mare5_3.dmd");
                    p.force_nearest = std::stoi(dargs.getd("force_nearest", "0"));
                    p.pos = fvec3_from_string(dargs.getd("pos", "0 0 0"));
                    p.scale = fvec3_from_string(dargs.getd("scale", "1 1 1"));
                    if (dargs.count("orn_eul") != 0){
                        const fvec3_t _angles = glm::radians(fvec3_from_string(dargs.getd("orn_eul", "0 0 0")));
                        p.orn = glm::eulerAngleXYZ(_angles.x, _angles.y, _angles.z);
                    }
                    auto* e = this->prtspawn(&p);
                    this->scene->set_tag(e, dargs.getd("tagname", ""));
                    return e;
                }
        };

        class EC_tagrotator : public Scene::EntityClass {
            using Scene::EntityClass::EntityClass;
            struct eparams_t {
                std::string targetname = "";
                fvec3_t axis = {0,0,1};
                fscalar_t rate = 45;
            };
            struct userdata_t {
                eparams_t p;
            };

            public:
                virtual std::string classname() {return "tagrotator";}
            private:
                static void _tagrotator_logic(Scene::entity& self, const gl45rw::MainloopData& mld) {
                    auto& p = ((userdata_t*)self.userdata)->p;
                    const std::string& tag = p.targetname;
                    if (self.scene->tagged_ents.count(tag) == 0){return;}
                    auto& entries = self.scene->tagged_ents[tag];
                    for (auto teid : entries) {
                        auto* te = self.scene->getent(teid);
                        te->trs.rotate(glm::normalize(p.axis), p.rate * mld.ticdelta);
                    }
                }
                static void _tagrotator_delete(Scene::entity& self){
                    auto* ud = (userdata_t*)self.userdata;
                    delete ud; self.userdata = nullptr;
                }
            protected:
                virtual Scene::entity* prtspawn(void* args) {
                    auto& params = *(eparams_t*)args;
                    auto* e = this->scene->spawn();
                    auto* ud = new userdata_t();
                    ud->p = params;
                    e->userdata = ud;
                    e->logic = _tagrotator_logic;
                    e->on_delete = _tagrotator_delete;
                    return e;
                }
                virtual Scene::entity* prtspawn_da(const dictionary<std::string, std::string>& dargs) {
                    eparams_t p;
                    p.targetname = dargs.getd("targetname", "");
                    p.axis = fvec3_from_string(dargs.getd("axis", "0 0 1"));
                    p.rate = std::stof(dargs.getd("rate", "45"));
                    return this->prtspawn(&p);
                }

        };

        struct machinedata_t {
            SpaceShooterMain* ei = nullptr;
            std::u32string
                dispname = U"?",
                name = U"?",
                mdlpath = U"/models/spaceship.dmd"
            ;
            SceneGraphModel* model = nullptr;
            fscalar_t
                topspeed[2],
                accel[2],
                normgrip[2],
                skidgrip[2],
                traction[2],
                brakepower[2]
            ;
            struct engfirespawner_t {
                fvec3_t lpos = {0,0,0};
                fmat33_t lorn = fmat33_t(1);
                fvec3_t partofsmin = {-0.25f, 0.0f, -0.25f}, partofsmax = {0.25f, 0.0f, 0.25f};
            };
            std::vector<engfirespawner_t> engine_fire_spawners = {};
            machinedata_t() {}
            machinedata_t(SpaceShooterMain* _ei, std::istream& ifs, const std::u32string& _name) : ei(_ei), name(_name){
                this->engine_fire_spawners.clear();
                while (ifs) {
                    std::string fieldname = "", fieldvalue = "";
                    #define _read_fpair(_attribn) ifs >> this->_attribn[0] >> this->_attribn[1]
                    djutil::ezstr::u8reads(fieldname, ifs);
                    if (fieldname.size() == 0){break;}
                    else if (djutil::ezstr::startswith<char>(fieldname, "//")){
                        char32_t tchr;
                        djutil::ezstr::u8skip_until(tchr, ifs, U"\n\r");
                    }
                    else if (fieldname == "DisplayName") {
                        this->dispname.clear();
                        djutil::ezstr::parse_strlit(this->dispname, ifs);
                    }
                    else if (fieldname == "ModelPath") {
                        this->mdlpath.clear();
                        djutil::ezstr::parse_strlit(this->mdlpath, ifs);
                    }
                    else if (fieldname == "EngineFireSpawner"){
                        engfirespawner_t efs;
                        while (true) {
                            std::string fieldname2 = "", fieldvalue2 = "";
                            djutil::ezstr::u8reads(fieldname2, ifs);
                            if (fieldname2.size() == 0){throw std::runtime_error("premature EOF in EngineFireSpawner field list!");}
                            else if (fieldname2.front() == ';'){break;}
                            else if (fieldname2 == "LocalPosition"){
                                ifs >> efs.lpos.x >> efs.lpos.y >> efs.lpos.z;
                            }
                            else if (fieldname2 == "LocalOrientation"){
                                fvec3_t eul = {0,0,0};
                                ifs >> eul.x >> eul.y >> eul.z;
                                efs.lorn = orn_from_eul(eul);
                            }
                            else if (fieldname2 == "ParticleOffsetMin"){
                                ifs >> efs.partofsmin.x >> efs.partofsmin.y >> efs.partofsmin.z;
                            }
                            else if (fieldname2 == "ParticleOffsetMax"){
                                ifs >> efs.partofsmax.x >> efs.partofsmax.y >> efs.partofsmax.z;
                            }
                            else {std::cout << "WARNING: unknown EngineFireSpawner field named " << std::quoted(fieldname2) << " in " << djutil::ezstr::utf32_to_utf8(this->name) << "\n";}

                        }
                        this->engine_fire_spawners.push_back(efs);
                    }
                    else if (fieldname == "TopSpeed") {_read_fpair(topspeed);}
                    else if (fieldname == "Acceleration") {_read_fpair(accel);}
                    else if (fieldname == "NormalGrip") {_read_fpair(normgrip);}
                    else if (fieldname == "SkidGrip") {_read_fpair(skidgrip);}
                    else if (fieldname == "Traction") {_read_fpair(traction);}
                    else if (fieldname == "BrakePower") {_read_fpair(brakepower);}
                    else {std::cout << "WARNING: unknown machine data field named " << std::quoted(fieldname) << " in " << djutil::ezstr::utf32_to_utf8(this->name) << "\n";}
                    #undef _read_fpair
                }
                this->model = this->ei->new_asset_rp<SceneGraphModel>();
                this->model->merge_model(this->mdlpath);
            }
            ~machinedata_t() {
                if (this->ei != nullptr && this->model != nullptr){
                    this->ei->del_asset(*(this->model));
                }
                this->model = nullptr;
                this->ei = nullptr;
            }
        };

        dictionary<std::u32string, machinedata_t*> machines = {};

        void load_machines() {
            djutil::virfs::fsdirlist_t files, dirs;
            auto& fs = this->I_GetMainFS()[0];
            fs.listdir(dirs, files, U"/machines/");
            for (auto filep : files) {
                std::cout << "load machines(): FILE -> " << djutil::ezstr::utf32_to_utf8(filep.fullname) << "\n";
                if (djutil::ezstr::endswith(filep.fullname, U".mac") || djutil::ezstr::endswith(filep.fullname, U".MAC")){
                    djutil::virfs::vfistream macistr(fs, filep.fullname);
                    auto* newmachine = new machinedata_t(this, macistr, filep.fullname);
                    this->machines[filep.fullname] = newmachine;
                    std::cout << "load_machines(): loaded " << djutil::ezstr::utf32_to_utf8(filep.fullname) << "\n";
                }
            }
        }

        void unload_machines(){
            for (auto p : this->machines) {
                delete p.second;
                p.second = nullptr;
            }
            this->machines.clear();
        }

        class EC_enginefire1 : public EntityClass {
            using EntityClass::EntityClass;
            public:
                SceneGraphModel* flaremdl = nullptr;
                Texture2D* flaretex0 = nullptr;

                struct eparams_t {
                    fvec3_t lpos = {0,0,0};
                    fscalar_t lifespan = 0.05f;
                    entid_t parent = 0;
                };
                struct userdata_t {
                    eparams_t p;
                    fscalar_t lifeclock = 0.0f;
                };

                virtual void onClassCreate() {
                    auto* ei = this->scene->instance;
                    /*
                    this->flaretex0 = ei->new_asset_rp<Texture2D>();
                    this->flaretex0->loadFromFS_A(U"/textures/fx/engflare1.png");

                    this->flaremdl = ei->new_asset_rp<SceneGraphModel>();
                    SceneGraphModel::merge_rawmeshes_list mrl;
                    mrl.emplace_back();
                    auto& ent0 = mrl.front();
                    ent0.trimeshproto.prim_unitrect_xy();
                    auto& mat0 = ent0.matproto;
                    mat0.diffuse.color = fvec3_t(1);
                    mat0.blendtype = matblendtype::add;
                    auto& m0t0 = mat0.texture_slots[0];
                    m0t0.use = true;
                    m0t0.texture = this->flaretex0[0];
                    m0t0.affects.diffuse_color = matprops_t::_mataffect<fvec3_t>{.use=true, .amount={1,1,1}};

                    this->flaremdl->merge_rawmeshes(mrl);
                    */
                }

                virtual std::string classname() {return "enginefire1";}

                virtual void onClassDestroy() {
                    /*
                    auto* ei = this->scene->instance;
                    ei->del_asset(this->flaremdl[0]);
                    ei->del_asset(this->flaretex0[0]);
                    this->flaremdl = nullptr;
                    this->flaretex0 = nullptr;
                    */
                }

                static void _engflare1_render(Scene::entity& self) {
                    auto* ei = reinterpret_cast<SpaceShooterMain*>(self.scene->instance);
                    //auto* mdl = ei->player_mdl;
                    auto* ud = reinterpret_cast<userdata_t*>(self.userdata);
                    auto* cls = reinterpret_cast<EC_enginefire1*>(self.scene->entity_classes[self.classname]);
                    fmat44_t mdlmat, viewmat, projmat; ei->I_GetMatrices(mdlmat, viewmat, projmat);
                    fvec3_t campos(viewmat * fvec4_t{0,0,0,1}), camup = glm::normalize(fvec3_t(viewmat * fvec4_t{0,1,0,0}));
                    SceneGraphModel::enque_args eargs;

                    auto* par = self.scene->getent(ud->p.parent);
                    if (par == nullptr){return;}

                    self.trs = par->trs;
                    self.trs.translate(ud->p.lpos, true);
                    self.trs.translate(fvec3_t{0.0f, -10.0f, 0.0f} * ud->lifeclock, true);

                    eargs.base_trs = self.trs;

                    eargs.base_trs.align_axis_to_vec(fvec3_t{0,1,0}, camup);
                    eargs.base_trs.align_axis_to_vec(fvec3_t{0,0,1}, glm::normalize(campos-self.trs.pos));

                    cls->flaremdl->enque(eargs);
                }

                static void _engflare1_physics_step(Scene::entity& self, const MainloopData& mld) {
                    auto* ud = (userdata_t*)self.userdata;
                }

                static void _engflare1_logic(Scene::entity& self, const MainloopData& mld) {
                    auto* ud = (userdata_t*)self.userdata;
                    auto* par = self.scene->getent(ud->p.parent);
                    if (par == nullptr){return;}
                    //self.trs.pos += (ud->wlinv * mld.ticdelta);

                    ud->lifeclock += mld.ticdelta;
                    if (par == nullptr || ud->lifeclock >= ud->p.lifespan){
                        self.scene->destroy(self.eid);
                        return;
                    }

                }

            protected:
                virtual entity* prtspawn(void* args) {
                    auto& params = *(eparams_t*)args;
                    auto* par = this->scene->getent(params.parent);
                    if (par == nullptr){return nullptr;}
                    auto* e = this->scene->spawn();
                    e->trs = par->trs;
                    e->trs.translate(params.lpos, true);
                    e->classname = this->classname();
                    auto* ud = new userdata_t;
                    ud->p = params;

                    ud->lifeclock = 0;
                    e->userdata = ud;
                    //e->on_delete = _player_destroy;
                    e->render = _engflare1_render;
                    e->logic = _engflare1_logic;
                    e->physics_step = _engflare1_physics_step;
                    return e;
                }
        };

        fvec3_t world_gravity;

        class EC_playership : public EntityClass {
            using EntityClass::EntityClass;
            public:

                SceneGraphModel* flaremdl = nullptr;
                Texture2D* flaretex0 = nullptr;

                //SoundBufferFactory* engsnd1_fac = nullptr;

                AutoPointer<AutoAsset<EngineInstance, SoundBufferFactory>> engsnd1_fac;
                AutoPointer<AutoAsset<EngineInstance, SoundBufferFactory>> boostsnd_fac;



                struct eparams_t {
                    int starting_health = 100;
                    fvec3_t pos = {0,0,0};
                    fmat33_t orn = fmat33_t(1);
                    std::u32string machine_data_name = U"/machines/default.mac";
                };
                struct efire1part_t {
                    fvec3_t clpos;
                    fscalar_t lifeclock = 0, lifespan = 0.125f, scale = 1.0f;
                };
                struct userdata_t {
                    eparams_t p;
                    int health;

                    fscalar_t
                        in_turn = 0,
                        in_throttle = 0,
                        in_brake = 0,
                        in_boost = 0,
                        in_pitch = 0,
                        in_strafe = 0,
                        setting = 1,

                        ytilt = 0,
                        xtilt = 0,
                        booster_cooldown_timer = 0,
                        engflare_add_timer = 0
                    ;
                    boolstate on_track_state;
                    fvec3_t wlinv = {0,0,0};

                    std::u32string machine_data_name = U"/machines/default.mac";
                    machinedata_t* machine_data = nullptr;
                    std::vector<efire1part_t> efire1parts = {}, efire1parts_keep = {};
                    std::vector<LightSource*> engine_fire_lights = {};
                    bool add_engflare1 = false, throttle_down = false, play_boostsnd = false;
                    SoundBufferHandle* engsnd1_hnd = nullptr;
                };

                virtual std::string classname() {
                    return "playership";
                }

                virtual void onClassCreate() {
                    auto* ei = this->scene->instance;

                    this->flaretex0 = ei->new_asset_rp<Texture2D>();
                    this->flaretex0->loadFromFS_A(U"/textures/fx/engflare1.png");

                    this->flaremdl = ei->new_asset_rp<SceneGraphModel>();
                    SceneGraphModel::merge_rawmeshes_list mrl;
                    mrl.emplace_back();
                    auto& ent0 = mrl.front();
                    ent0.trimeshproto.prim_unitrect_xy();
                    auto& mat0 = ent0.matproto;
                    mat0.diffuse.color = fvec3_t(1);
                    mat0.blendtype = matblendtype::add;
                    auto& m0t0 = mat0.texture_slots[0];
                    m0t0.use = true;
                    m0t0.texture = this->flaretex0[0];
                    m0t0.affects.diffuse_color = matprops_t::_mataffect<fvec3_t>{.use=true, .amount={1,1,1}};

                    this->flaremdl->merge_rawmeshes(mrl);

                    //this->engsnd1_fac = ei->new_asset_rp<SoundBufferFactory>();
                    //this->engsnd1_fac->loadFromFS(U"/sound/engine1.pcm");

                    this->engsnd1_fac(ei, ei->new_asset<SoundBufferFactory>());
                    this->boostsnd_fac(ei, ei->new_asset<SoundBufferFactory>());

                    this->engsnd1_fac->assetptr()->loadFromFS(U"/sound/engine3.pcm");
                    this->boostsnd_fac->assetptr()->loadFromFS(U"/sound/boost_w_cat.pcm");

                }
                virtual void onClassDestroy() {
                    auto* ei = this->scene->instance;

                    ei->del_asset(this->flaremdl[0]);
                    ei->del_asset(this->flaretex0[0]);
                    this->flaremdl = nullptr;
                    this->flaretex0 = nullptr;
                    /*
                    ei->del_asset(this->engsnd1_fac[0]);
                    this->engsnd1_fac = nullptr;
                    */
                }
            private:
                static void _player_clear_enginelights(Scene::entity& self) {
                    auto* ei = self.scene->instance;
                    auto* ud = reinterpret_cast<userdata_t*>(self.userdata);
                    for (auto*& last : ud->engine_fire_lights){
                        ei->del_asset(*last); last = nullptr;
                    }
                    ud->engine_fire_lights.clear();
                }
                static void _player_destroy(Scene::entity& self) {
                    auto* ei = self.scene->instance;
                    auto* ud = reinterpret_cast<userdata_t*>(self.userdata);
                    _player_clear_enginelights(self);
                    ei->del_asset(ud->engsnd1_hnd[0]);
                    ud->engsnd1_hnd = nullptr;
                    delete ud; ud = nullptr;
                    self.userdata = nullptr;
                }
                static void _player_render(Scene::entity& self) {
                    auto* ei = reinterpret_cast<SpaceShooterMain*>(self.scene->instance);
                    auto* cls = (EC_playership*)self.scene->entity_classes[self.classname];
                    //auto* mdl = ei->player_mdl;
                    auto* ud = reinterpret_cast<userdata_t*>(self.userdata);
                    if (ud->machine_data == nullptr){return;}
                    auto* mdl = ud->machine_data->model;
                    if (mdl == nullptr){return;}

                    fmat44_t mdlmat, viewmat, projmat; ei->I_GetMatrices(mdlmat, viewmat, projmat);
                    fvec3_t campos(viewmat * fvec4_t{0,0,0,1}), camup = glm::normalize(fvec3_t(viewmat * fvec4_t{0,1,0,0}));

                    trs_t mdltrs = self.trs;
                    mdltrs.rotate(fvec3_t{0,1,0}, ud->ytilt, true);
                    mdltrs.rotate(fvec3_t{1,0,0}, ud->xtilt, true);
                    SceneGraphModel::enque_args ea = {
                        .base_trs=mdltrs,
                        .rgrpmask=Scene::SCENE_WORLDMODEL
                    };
                    mdl->enque(ea);
                    /*
                    for (int i = 0; i < 4; i++){
                        SceneGraphModel::enque_args ea2 = {
                            .base_trs=mdltrs,
                            .rgrpmask=Scene::SCENE_WORLDMODEL
                        };
                        ea2.base_trs.translate(fvec3_t{
                            glm::mix(-1.0f, 1.0f, ei->I_GetRandomFloat()),
                            glm::mix(-3.0f, -2.0f, ei->I_GetRandomFloat()),
                            glm::mix(-1.0f, 1.0f, ei->I_GetRandomFloat())
                        }, true);
                        ea2.base_trs.scale = fvec3_t(0.25f);
                        cls->flaremdl->enque(ea2);
                    }
                    */
                    bool efls_on = (ud->in_brake <= 0.0f && ud->in_throttle > 0.0f);
                    for (size_t i = 0; i < ud->machine_data->engine_fire_spawners.size(); i++) {
                        auto& efs = ud->machine_data->engine_fire_spawners[i];
                        auto*& efl = ud->engine_fire_lights[i];
                        if (efls_on){
                            LightSource::addparams_t lap;
                            lap.pos = mdltrs.pos + mdltrs.getaxisvect(efs.lpos + fvec3_t{0,-1.5f,0});
                            lap.orn = mdltrs.orn;
                            efl->data.energy = glm::mix(10.0f, 15.0f, ei->I_GetRandomFloat());
                            //efl->addlight(lap);
                        }
                    }
                    for (auto& ef1p : ud->efire1parts) {
                        SceneGraphModel::enque_args ea2 = {
                            .base_trs=mdltrs,
                            .rgrpmask=Scene::SCENE_WORLDMODEL
                        };
                        ea2.base_trs.translate(ef1p.clpos, true);
                        ea2.base_trs.align_axis_to_vec(fvec3_t{0,0,1}, glm::normalize(ea2.base_trs.pos - campos));
                        ea2.base_trs.scale = fvec3_t(ef1p.scale);
                        cls->flaremdl->enque(ea2);
                    }

                }
                static void _player_physics_step(Scene::entity& self, const MainloopData& mld) {
                    auto* ei = reinterpret_cast<SpaceShooterMain*>(self.scene->instance);
                    auto* ud = (userdata_t*)self.userdata;
                    auto* scene = self.scene;
                    fscalar_t nytilt = 0;
                    //fvec3_t gravity = {0,0,-125.0f};
                    fvec3_t gravity = ei->world_gravity;
                    Scene::trace1_result trackres;
                    if (!ud->on_track_state.current){
                        ud->xtilt += (90.0f * ud->in_pitch * mld.ticdelta);
                        ud->xtilt = glm::clamp(ud->xtilt, -65.0f, 65.0f);
                    }
                    else {
                       ud->xtilt = 0;
                    }
                    trs_t xtilt_trs; xtilt_trs.rotate(fvec3_t{1,0,0}, ud->xtilt);
                    fvec3_t llinv = EC_playership::get_llinv(&self);
                    //fvec3_t rayendpos = self.trs.pos + (ud->wlinv * mld.ticdelta) + self.trs.getaxisvect(fvec3_t{0,0,-5.0f});
                    fvec3_t rayendpos = self.trs.pos + self.trs.getaxisvect(fvec3_t{0,0,(-7.5f) - glm::max(-llinv.z * mld.ticdelta, 0.0f)});
                    bool on_track = scene->trace1(trackres, self.trs.pos, rayendpos, uint32_t(-1), true);
                    ud->on_track_state = on_track;
                    bool zero_zvel = false, grav2track = false;
                    fvec3_t trackgravdir;
                    fmat33_t tilted_mat = xtilt_trs.orn, tilted_mat_inv = glm::inverse(tilted_mat);
                    fvec3_t exhaust_flare_vector = glm::normalize(self.trs.getaxisvect(tilted_mat * fvec3_t{0,-1,0}));
					fvec3_t exhaust_flare_side_vector = glm::normalize(self.trs.getaxisvect(tilted_mat * fvec3_t{1,0,0}));
                    bool add_engflare1 = false;
                    if (on_track){
                        /*
                        fvec3_t newpos = trackres.hitpos + (trackres.hitnormal * 2.0f);

                        if (trackres.hitdist > 2.0f){self.trs.pos = _mathstuff::gmi(self.trs.pos, newpos, fvec3_t(0.01f), mld.ticdelta);}
                        else {self.trs.pos = newpos;}
                        */
                        fscalar_t hoverdist = 2;
                        self.trs.align_axis_to_vec(fvec3_t{0,0,1}, trackres.hitnormal_smoothed);

                        fscalar_t ddelta = hoverdist - trackres.hitdist;
                        fvec3_t tl = ddelta * trackres.hitnormal_smoothed;
                        zero_zvel = true;

                        if (ud->on_track_state.state() == 2){
                            self.trs.pos = trackres.hitpos + (trackres.hitnormal_smoothed * hoverdist);
                        }
                        else {
                            self.trs.translate(tl);
                        }

                        gravity = fvec3_t(0);
                    }
                    else {
                        self.trs.align_axis_to_vec(fvec3_t{0,0,1}, fvec3_t{0,0,1}, 0.98f, mld.ticdelta);
                    }
                    if (ud->in_turn != 0) {
                        self.trs.rotate(fvec3_t{0,0,1}, 90.0f * ud->in_turn * mld.ticdelta, true);
                        //nytilt = -22.5f * ud->in_turn;
                        nytilt = -10.0f * ud->in_turn;
                    }
                    ud->ytilt = _mathstuff::gmi(ud->ytilt, nytilt, 0.025f, mld.ticdelta);

                    auto* md = ud->machine_data;
                    llinv = EC_playership::get_llinv(&self);
                    fvec3_t llinv_tilted = tilted_mat_inv * llinv;
                    #define _lerpstat(_sn) glm::mix(md->_sn[0], md->_sn[1], ud->setting)
                    fscalar_t
                        topspeed = _lerpstat(topspeed) * SPEED_SCALE,
                        accel = _lerpstat(accel) * SPEED_SCALE,
                        normgrip = _lerpstat(normgrip),
                        brakepower = _lerpstat(brakepower),
                        curgrip = normgrip
                    ;
                    ud->booster_cooldown_timer = glm::max(ud->booster_cooldown_timer - mld.ticdelta, 0.0f);
                    fscalar_t& cyvel = llinv_tilted.y;
                    //ud->engflare_add_timer = glm::max(ud->engflare_add_timer - mld.ticdelta, 0.0f);
                    ud->throttle_down = false;
                    if (ud->in_throttle > 0.0f){
                        ud->throttle_down = true;
                        if (ud->booster_cooldown_timer > 0.1f){
                            accel = accel*2.0f;
                            //topspeed = topspeed + 120.0f;
                            topspeed = topspeed + (533.0f * SPEED_SCALE);
                        }
                        else if (ud->booster_cooldown_timer < FLT_EPSILON && (ud->in_boost > 0.5f)) {
                            ud->booster_cooldown_timer = 2.0f;
                            ud->play_boostsnd = true;
                        }

                        if (!ud->on_track_state.current && ud->xtilt < 0){
                            topspeed = 9999.0f;
                        }

                        if (cyvel <= topspeed){
                            fscalar_t accel2 = accel, speedfrac = cyvel / topspeed, cyvel_target;
                            if (speedfrac >= 0.75f){
                                accel2 = glm::mix(accel, accel*0.0001f, (speedfrac - 0.75f)/(1.0f - 0.75f));
                            }
                            cyvel_target = cyvel + (accel2 * ud->in_throttle * mld.ticdelta);

                            cyvel = glm::min(topspeed, cyvel_target);
                        }
                        else {
                            cyvel = _mathstuff::gmi(cyvel, topspeed, 0.5f, mld.ticdelta);
                        }



                    }
                    else if (ud->in_brake > 0.0f){
                        cyvel = _mathstuff::gmi(cyvel, 0.0f, 1.0f - (brakepower * ud->in_brake), mld.ticdelta);
                    }
                    else {
                        cyvel = _mathstuff::gmi(cyvel, 0.0f, 0.95f, mld.ticdelta);
                    }
                    llinv = tilted_mat * llinv_tilted;
                    llinv.x = _mathstuff::gmi(llinv.x, 0.0f, 1.0f - curgrip, mld.ticdelta);

                    if (on_track){
                        if (!grav2track){
                            //llinv.z = glm::max(_mathstuff::gmi(llinv.z, 0.0f, 0.75f, mld.ticdelta), 0.0f);
                            llinv.z = 0;
                            gravity = fvec3_t(0);
                        }
                        else {
                            gravity = (trackgravdir * 500.0f);
                        }
                    }
                    else {
                        //ud->xtilt += (90.0f * ud->in_pitch * mld.ticdelta);
                        //ud->xtilt = glm::clamp(ud->xtilt, -65.0f, 65.0f);
                        if (ud->xtilt < 0.0f){
                            //llinv.y = _mathstuff::gmi(llinv.y, 0.0f, 0.1f, mld.ticdelta);
                            //llinv.z -= (125.0f * (ud->xtilt / -65.0f) * mld.ticdelta);
                        }
                    }

                    EC_playership::set_llinv(&self, llinv);
                    ud->wlinv += (gravity * mld.ticdelta);
                    self.trs.pos += (ud->wlinv * mld.ticdelta);
                    if (add_engflare1){
                        //EC_enginefire1::eparams_t ehp;
                        //ehp.pos = self.trs.pos + (exhaust_flare_vector * 2.0f) + (exhaust_flare_side_vector * (ei->I_GetRandomFloat() - 0.5f));
                        //ehp.lpos = fvec3_t{glm::mix(fscalar_t(-1), fscalar_t(1), ei->I_GetRandomFloat()),-2,0};
                        //ehp.parent = self.eid;
                        //ehp.wlinv = ud->wlinv + (exhaust_flare_vector * 200.0f);
                        //auto* ehpe = scene->spawn("enginefire1", &ehp);
                        //ehpe->trs.scale = fvec3_t(2.5f);
                        //ehpe->egroup = self.egroup;
                        //auto* ef1p = new efire1part_t;
                        //ef1p->clpos = fvec3_t{glm::mix(fscalar_t(-1), fscalar_t(1), ei->I_GetRandomFloat()),-2,0};
                        //ef1p->lifeclock = 0.0f;
                        //ud->efire1parts.push_back(ef1p);
                    }
                    ud->add_engflare1 = add_engflare1;


                }
                static void _player_logic(Scene::entity& self, const MainloopData& mld) {
                    auto* ei = reinterpret_cast<SpaceShooterMain*>(self.scene->instance);
                    auto* ud = (userdata_t*)self.userdata;
                    auto* scene = self.scene;
                    auto* cls = scene->classof<EC_playership>(&self);
                    auto* md = ud->machine_data;
                    fvec3_t llinv = get_llinv(&self);
                    #define _lerpstat(_sn) glm::mix(md->_sn[0], md->_sn[1], ud->setting)
                    fscalar_t
                        topspeed = _lerpstat(topspeed) * SPEED_SCALE,
                        accel = _lerpstat(accel) * SPEED_SCALE,
                        normgrip = _lerpstat(normgrip),
                        brakepower = _lerpstat(brakepower),
                        curgrip = normgrip
                    ;
                    #undef _lerpstat

                    ud->efire1parts_keep.clear();
                    for (auto& ef1p : ud->efire1parts){
                        ef1p.lifeclock += mld.ticdelta;
                        if (ef1p.lifeclock >= ef1p.lifespan){continue;}
                        ud->efire1parts_keep.push_back(ef1p);
                    }
                    ud->efire1parts.clear();
                    ud->efire1parts.insert(ud->efire1parts.end(), ud->efire1parts_keep.begin(), ud->efire1parts_keep.end());
                    ud->add_engflare1 = false;
                    ud->engflare_add_timer -= mld.ticdelta;
                    if (ud->engflare_add_timer < FLT_EPSILON){
                        ud->engflare_add_timer = 0.00175f;
                        ud->add_engflare1 = true;
                    }
                    if (ud->throttle_down && ud->add_engflare1){
                        for (auto& efs : md->engine_fire_spawners){
                            ud->efire1parts.emplace_back();
                            auto& ef1p = ud->efire1parts.back();
                            fscalar_t base_lifespan = glm::mix(0.01f, 0.025f, glm::clamp(llinv.y / topspeed, 0.0f, 2.0f));
                            /*
                            if (ud->booster_cooldown_timer > 0.0f){
                                //fscalar_t sine = glm::clamp(glm::sin(glm::mix(0.0f, 3.14519f, (ud->booster_cooldown_timer / 2.0f))), 0.0f, 1.0f);
                                //base_lifespan *= (1.0f + sine);
                                base_lifespan = glm::mix(0.01f, 0.075f, glm::clamp(llinv.y / topspeed, 0.0f, 2.0f));
                            }
                            */
                            ef1p.lifespan = base_lifespan * glm::mix(1.0f, 1.5f, ei->I_GetRandomFloat());
                            //if (ud->booster_cooldown_timer > 0.0f){ef1p.lifespan *= glm::mix(2.0f, 1.0f, (ud->booster_cooldown_timer / 2.0f));}
                            ef1p.scale = glm::mix(0.5f, 1.0f, ei->I_GetRandomFloat());
                            ef1p.clpos = efs.lpos + glm::mix(efs.partofsmin, efs.partofsmax, fvec3_t{ei->I_GetRandomFloat(),ei->I_GetRandomFloat(),ei->I_GetRandomFloat()});
                        }
                    }
                    for (auto& ef1p : ud->efire1parts){
                        ef1p.clpos += fvec3_t{0,-64/2,0} * mld.ticdelta;
                    }
                    if (ud->engsnd1_hnd != nullptr && ud->engsnd1_hnd->get_state() != EngineInstance::soundhandlestate::stopped){
                        fscalar_t curspd = glm::length(ud->wlinv);
                        fscalar_t pitchfac = glm::mix(0.2f, 1.0f, glm::clamp(curspd / topspeed, 0.0f, 1.5f));
                        ud->engsnd1_hnd->set_pitch(pitchfac);
                    }
                    if (ud->play_boostsnd){
                        ud->play_boostsnd = false;
                        uid_t hnd = ei->S_PlaySound(cls->boostsnd_fac->assetptr(), EngineInstance::playsoundargs_t{.gain=1.0f});
                    }
                }
            public:
                static int set_machine_data(Scene::entity* e, const std::u32string& newname){
                    if (e == nullptr){return -1;}
                    if (newname == U""){return -2;}
                    if (e->classname != "playership"){return -3;}
                    auto* ei = (SpaceShooterMain*)(e->scene->instance);
                    auto* scene = e->scene;

                    if (ei->machines.count(newname) == 0){return -4;}
                    _player_clear_enginelights(*e);
                    auto* data = ei->machines[newname];
                    auto* ud = (userdata_t*)e->userdata;

                    ud->machine_data_name = newname;
                    ud->machine_data = data;

                    for (auto& efs : data->engine_fire_spawners){
                        lightprops_t lp;
                        lp.radius = 5.0f;
                        lp.energy = 10.0f;
                        lp.diffuse.color = fvec3_t{0,1,1};
                        ud->engine_fire_lights.push_back(ei->new_asset_rp<LightSource>(lp));
                    }

                    return 0;
                }
                static fvec3_t get_llinv(Scene::entity* e) {
                    auto* ud = (userdata_t*)(e->userdata);
                    fmat33_t orninv = glm::inverse(e->trs.orn);
                    return orninv * ud->wlinv;
                }
                static void set_llinv(Scene::entity* e, const fvec3_t& newllinv){
                    auto* ud = (userdata_t*)(e->userdata);
                    ud->wlinv = e->trs.orn * newllinv;
                }
            protected:
                virtual entity* prtspawn(void* args) {
                    auto& params = *(eparams_t*)args;
                    auto* e = this->scene->spawn();
                    e->trs.pos = params.pos;
                    e->trs.orn = params.orn;

                    e->classname = this->classname();
                    auto* ud = new userdata_t;
                    ud->p = params;
                    ud->health = params.starting_health;

                    uid_t engsnd1_hndidx = this->scene->instance->S_PlaySound(this->engsnd1_fac->assetptr(), EngineInstance::playsoundargs_t{.loop=true});
                    ud->engsnd1_hnd = this->scene->instance->get_asset<SoundBufferHandle>(engsnd1_hndidx);
                    ud->engsnd1_hnd->set_gain(0.25f);
                    //ud->engsnd1_hnd->set_gain(0.5f);
                    ud->engsnd1_hnd->set_pitch(0.2f);

                    e->userdata = ud;
                    e->on_delete = _player_destroy;
                    e->render = _player_render;
                    e->logic = _player_logic;
                    e->physics_step = _player_physics_step;
                    set_machine_data(e, params.machine_data_name);
                    return e;
                }
                virtual entity* prtspawn_da(const Scene::dargs_t& dargs) {
                    eparams_t p;
                    p.starting_health = std::stoi(dargs.getd("starting_health", "100"));
                    p.pos = fvec3_from_string(dargs.getd("pos", "0 0 0"));
                    p.orn = orn_from_eul(dargs.getd("orn_eul", "0 0 0"));
                    p.machine_data_name = djutil::ezstr::utf8_to_utf32(dargs.getd("machine_data_name", "/machines/default.mac"));
                    return this->prtspawn(&p);
                }
        };

        struct params_t {
            std::string basedir = "base";
            bool swmode = false;
            int renderscale = 100;
            int width = 640, height = 480;
            bool fullscreen = false;
            std::string mapname = "firefield";
            std::string machine = "/machines/boatmobile1.mac";
            std::vector<std::string> mods = {};
        };
        //dictionary<char32_t, BitmapFont::charmap_entry> fzgx_speedo_charmap = {};
        BitmapFont* fzgx_speedo_font = nullptr;
        Scene::ParticleSystem* test_psys1 = nullptr;
        StopWatch test_psys1_spawnclock = {.interval=0.05f};

        struct bgmtrack_t {
            AutoPointer<AutoAsset<SpaceShooterMain, SoundBufferFactory>> factory;
            fscalar_t gain = 0.5f;
            std::vector<fclock_t> final_lap_markers;
        };

        bgmtrack_t main_bgm = {}, final_lap_bgm = {};

        int bgm_state = 0;
        bool bgm_pending_final_lap = false;

        fclock_t final_lap_bgm_nextmarker = 0;

        SoundBufferHandle* music_hnd = nullptr;
        uid_t music_hnd_idx = 0;

        class ResetPlayerCCMD : public ConsoleCommand {
            using ConsoleCommand::ConsoleCommand;
            public:
                virtual int invoke(const std::u32string& args, CCMDInvokationExtraData& edata) {
                    auto* ei = (SpaceShooterMain*)(this->instance);
                    auto* player = ei->main_scene->getent(ei->player_eid);
                    if (player != nullptr){
                        auto* pud = (EC_playership::userdata_t*)player->userdata;
                        pud->wlinv = fvec3_t(0);
                        player->trs = trs_t();
                    }
                    return 0;
                }
        };


        void stop_bgm() {
            if (this->music_hnd_idx){this->S_StopSound(this->music_hnd_idx);}
            //if (this->S_IsHandle(this->music_hnd_idx)){this->del_asset(this->music_hnd_idx);}
            this->music_hnd = nullptr;
            ~(this->main_bgm.factory);
            ~(this->final_lap_bgm.factory);
            this->bgm_state = 0;
            this->bgm_pending_final_lap = false;
            this->music_hnd_idx = 0;
        }
        void set_bgm(const std::u32string& fsp) {
            this->stop_bgm();

            std::stringstream bss; this->I_GetMainFS()->open(bss, fsp);

            bool has_mainbgm = false, has_finalbgm = false;

            while (bss) {
                std::string cmdname; djutil::ezstr::u8reads(cmdname, bss);
                std::cout << cmdname << "\n";
                if (cmdname.size() == 0){break;}
                else if (cmdname == "file1") {
                    std::u32string fsp; djutil::ezstr::parse_strlit(fsp, bss);
                    this->main_bgm.factory(this, this->new_asset<SoundBufferFactory>());
                    this->main_bgm.factory->assetptr()->loadFromFS(U"/music/" + fsp);
                    has_mainbgm = true;
                }
                else if (cmdname == "file2") {
                    std::u32string fsp; djutil::ezstr::parse_strlit(fsp, bss);
                    this->final_lap_bgm.factory(this, this->new_asset<SoundBufferFactory>());
                    this->final_lap_bgm.factory->assetptr()->loadFromFS(U"/music/" + fsp);
                    has_finalbgm = true;
                }
                else if (cmdname == "gain1") {
                    bss >> this->main_bgm.gain;
                }
                else if (cmdname == "gain2") {
                    bss >> this->final_lap_bgm.gain;
                }
            }
            if (has_mainbgm){
                this->bgm_state = 1;
            }
        }

        void update_bgm() {
            int curtrack = 0;
            //auto* mainfac = this->main_bgm.factory->assetptr();
            //auto* finalfac = this->final_lap_bgm.factory->assetptr();
            /*
            if (this->music_hnd != nullptr){
                if (this->music_hnd->factory == mainfac && mainfac != nullptr){curtrack = 1;}
                else if (this->music_hnd->factory == finalfac && finalfac != nullptr){curtrack = 2;}
            }
            */
            switch (this->bgm_state) {
                case 1: {
                    this->music_hnd_idx = this->S_PlaySound(this->main_bgm.factory->assetptr(), EngineInstance::playsoundargs_t{.loop=true, .keep=true, .gain=this->main_bgm.gain});
                    this->music_hnd = this->get_asset<SoundBufferHandle>(this->music_hnd_idx);
                    this->bgm_state = 2;
                    break;
                }
                case 2: {
                    if (this->bgm_pending_final_lap){
                        this->bgm_pending_final_lap = false;
                        if (this->final_lap_bgm.factory){
                            this->final_lap_bgm_nextmarker = this->music_hnd->duration;
                            this->bgm_state = 3;
                            fclock_t curpos = this->music_hnd->tell();
                            this->music_hnd->set_looping(false);
                            for (auto& m : this->main_bgm.final_lap_markers) {
                                if (curpos < m){this->final_lap_bgm_nextmarker = m; break;}
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    fclock_t curpos = this->music_hnd->tell();
                    if (curpos >= this->final_lap_bgm_nextmarker){
                        this->S_StopSound(*this->music_hnd);
                        this->del_asset(this->music_hnd_idx);

                        this->music_hnd_idx = this->S_PlaySound(this->final_lap_bgm.factory->assetptr(), EngineInstance::playsoundargs_t{.loop=true, .keep=false, .gain=this->final_lap_bgm.gain});
                        this->music_hnd = this->get_asset<SoundBufferHandle>(this->music_hnd_idx);

                        this->bgm_state = 4;
                        break;
                    }
                    break;
                }
                case 4: {break;}
                default: ;
            }
        }

        void setup_speedo_font() {
            //this->fzgx_speedo_charmap.clear();
            this->fzgx_speedo_font = this->new_asset_rp<BitmapFont>();
            this->fzgx_speedo_font->load_from_file(U"/textures/hud/fzgx_speedo.png");
            auto* tex = this->fzgx_speedo_font->font_texture;
            tex->_minfilter = tex->_magfilter = texfilter::nearest;
            this->fzgx_speedo_font->character_map.clear();
            for (int digit = 0; digit < 10; digit++){
                int row = digit/5, col = digit - (row * 5);
                char32_t digit_c = (U'0' + digit);
                BitmapFont::charmap_entry cment;
                cment.tluv = fvec2_t{col*0.2f, 1.0f - (row / 4.0f)};
                cment.bruv = cment.tluv + fvec2_t{0.2f, -0.25f};
                std::cout << row << ", " << col << "\n";
                this->fzgx_speedo_font->character_map[digit_c] = cment;
            }
            this->fzgx_speedo_font->character_map[U'K'] = BitmapFont::charmap_entry{
                .tluv={0.0f, 28.0f/64.0f},
                .bruv={28.0f/120.0f, 0.0f}
            };
        }


        virtual void _onCreate(void* params){
            auto p = (params != nullptr ? *(params_t*)params : params_t{});

            std::vector<std::u32string> srcs = {
                ezstr::utf8_to_utf32(p.basedir)
            };

            for (auto& m8 : p.mods) {
                srcs.push_back(ezstr::utf8_to_utf32(m8));
            }

            this->I_StartMainFS(srcs);

            this->default_bindcat = this->I_CreateBindCategory("_default", gl45rw::bindcmdnames_t{}, EngineInstance::bindcatflags(0U));


            const std::array<std::u32string, 9> _ivar_names = {
                U"_testleft",
                U"_testright",
                U"_testup",
                U"_testdown",
                U"_testzoomout",
                U"_testzoomin",
                U"_testboost",
                U"_pitchup",
                U"_pitchdown"
            };

            for (auto _ivn : _ivar_names) {
                this->new_ccmd<gl45rw::ConsoleIVAR>(_ivn);
            }

            this->new_ccmd<ResetPlayerCCMD>(U"reset_player");

            this->C_ExecBatchFileFS(U"/game.rc");

            this->player_mdl = this->new_asset_rp<SceneGraphModel>();
            //this->player_mdl->merge_model(U"/models/spaceship.dmd");
            //this->player_mdl->merge_model(U"/models/toytimesky.dmd");
            this->setup_speedo_font();

            this->load_machines();

            this->main_scene = new Scene(this);

            this->main_scene->num_physics_steps = 5;
            this->main_scene->num_logic_steps = 10;



            /*
            auto* tl0 = this->main_scene->spawn("light", Scene::dargs_t{
                {"pos", "0 0 10"},
                {"type", "hemi"}
            });
            */


            this->R_SetVideoMode(videomode_t{
                .renderer=(p.swmode ? gfxapi::sw3d3 : gfxapi::opengl),
                .width=p.width,
                .height=p.height,
                .sw3d2_resolution_percent=p.renderscale,
                .fullscreen=p.fullscreen,
                .opengl_resolution_percent=p.renderscale,
                .sw3d3_resolution_percent=p.renderscale,
            });
            #ifdef GL45RW_WITH_OPENAL
            this->S_SetSoundMode(soundmode_t{
                .renderer=sndapi::openal
            });
            #endif

            this->main_scene->newclass<EC_playership>();
            this->main_scene->newclass<EC_light>();
            this->main_scene->newclass<EC_mapmodel>();
            this->main_scene->newclass<EC_tagrotator>();
            this->main_scene->newclass<EC_enginefire1>();

            //this->test_psys1(this->main_scene);
            this->test_psys1 = this->main_scene->psys_new_rp();

            int ptmp1_idx;
            auto& ptmp1 = this->test_psys1->getptmp(ptmp1_idx = this->test_psys1->new_template());

            //this->test_psys1->ptmp_mergeSGModel(ptmp1_idx, U"/models/testpart1.dmd");
            this->test_psys1->ptmp_loadSprite(ptmp1_idx, U"/textures/fx/engflare1.png", Scene::loadspr_args{.scale=fvec3_t{2,2,2}});
            //ptmp1.lifespan = 1.0f;
            //ptmp1.linv = fvec3_t{0,0,10};
            ptmp1.gravity = fvec3_t{0,0,-125.0f * (SPEED_SCALE / 0.2f)};

            ptmp1.ptmp_linvs[0] = fvec3_t{-10,-10,100};
            ptmp1.ptmp_linvs[1] = fvec3_t{10,10,100};

            ptmp1.ptmp_lifespans[0] = 1.0f;
            ptmp1.ptmp_lifespans[1] = 1.5f;



            this->setmap1(U"/maps/" + ezstr::utf8_to_utf32(p.mapname) + U".map", p.machine);
        }

        static int _map_on_extracmd(Scene::worldfile_extracmdargs_t& args) {
            auto* ei = (SpaceShooterMain*)(args.scene->instance);
            if (args.cmdname_u8 == "bgm") {
                std::u32string fsp; djutil::ezstr::parse_strlit(fsp, *args.wfss_ptr);
                ei->set_bgm(fsp);
            }
            else if (args.cmdname_u8 == "gravity"){
                fvec3_t grav; args.wfss_ptr[0] >> grav.x >> grav.y >> grav.z;
                ei->world_gravity = grav * (SPEED_SCALE / 0.2f);
            }
            return 0;
        }
        void setmap1(const std::u32string& mapname, const std::string& machine_name) {
            this->world_gravity = fvec3_t{0,0,-200.0f * (SPEED_SCALE / 0.2f)};
            this->main_scene->spawn_worldfile(mapname, 1, _map_on_extracmd);
            auto* player = this->main_scene->spawn("playership", Scene::dargs_t{
                {"machine_data_name", machine_name}
            });
            this->player_eid = player->eid;
        }

        virtual void _onWindowCloseEvent() {this->_mustclose = true;}
        float getinval(const std::string& iname) {
            if (int(this->console_data.state) != 0){return 0.0f;}
            return this->E_GetInputValue(iname);
        }
        boolstate con_oc_bs;

        fscalar_t zangle = 0, xangle = 67.5f, camdist = 9.5f;

        virtual bool update(const gl45rw::MainloopData& mld) {
			if (this->_mustclose){return false;}
			if ((this->getinval("escape")) > 0.5f){return false;}
			//std::cout << "CHECKED ESC\n";
            bool contoggle = ((this->con_oc_bs = (this->E_GetInputValue("tilde") > 0.5f)) == 2);
            //std::cout << "CHECKED TILDE\n";
            if (this->C_IsConsoleOpen() && contoggle){
                this->C_SetConsoleOpen(false);
            }
            else if (!this->C_IsConsoleOpen() && contoggle) {
                this->C_SetConsoleOpen(true);
            }

            this->update_bgm();

            if (this->test_psys1_spawnclock += mld.ticdelta){
                for (int i = 0; i < 10; i++){
                    auto* p = this->test_psys1->emit_particle(trs_t());
                    //p->lifespan = glm::mix(0.75f, 1.25f, this->I_GetRandomFloat());
                }
            }

            auto* player = this->main_scene->getent(this->player_eid);
            if (player != nullptr){
                //player->trs.rotate(fvec3_t{0,0,1}, 20.0f * mld.ticdelta);
            }
            this->I_UpdateAllBindings();
            //std::cout << "UPDATED ALL BINDINGS\n";
			this->C_UpdateConsole(mld);
			//std::cout << "UPDATED CONSOLE\n";
			this->main_scene->update(mld);
			//std::cout << "UPDATED SCENE\n";
			bool do_render = true;
			//auto& ivars = this->ivar_ccmds;
            //this->zangle += (this->I_GetIVARValue(U"_testleft", U"_testright") * 40.0f * mld.ticdelta);
            //this->xangle += (this->I_GetIVARValue(U"_testup", U"_testdown") * 40.0f * mld.ticdelta);
            this->camdist += (this->I_GetIVARValue(U"_testzoomout", U"_testzoomin") * 5.0f * mld.ticdelta);
            //std::cout << "POLLED CAMDIST IVARS\n";
            EC_playership::userdata_t* pud = nullptr;
            if (player != nullptr){
                pud = (EC_playership::userdata_t*)(player->userdata);
                pud->in_throttle = this->I_GetIVARValue(U"_testup", U"_testdown");
                pud->in_brake = this->I_GetIVARValue(U"_testdown", U"_testup");
                pud->in_turn = this->I_GetIVARValue(U"_testleft", U"_testright");
                pud->in_pitch = this->I_GetIVARValue(U"_pitchup", U"_pitchdown");
                pud->in_boost = this->ivar_ccmds[U"_testboost"]->_value;
            }
            if (do_render){
                //std::cout << "MAKING CURRENT\n";
                this->R_MakeCurrent();
                //std::cout << "MADE CURRENT\n";
                this->R_SetViewportFromWindow();
                this->R_SetClearColor(fvec4_t(0.0625f));
                this->R_ClearBuffers(true, true);

                this->matrices.model.loadIdentity();
                this->matrices.view.loadIdentity();
                /*
                 this->matrices.view.rotate(fvec3_t{0,0,1}, this->zangle);
                 this->matrices.view.rotate(fvec3_t{1,0,0}, this->xangle);
                 this->matrices.view.translate(fvec3_t{0.0f,0.0f,this->camdist});
                */
                fscalar_t cam_fov = 90.0f;
                fscalar_t plspeed = 0.0f;


                if (player != nullptr){
                    fvec3_t plinv = EC_playership::get_llinv(player);
                    cam_fov = glm::mix(90.0f, 130.0f, glm::clamp(plinv.y/(360.0f * (SPEED_SCALE / 0.2f)), 0.0f, 1.0f));
                    this->matrices.view[-1] = fmat44_t(player->trs);
                    this->matrices.view.rotate(fvec3_t{1,0,0}, pud->xtilt);
                    this->matrices.view.translate(fvec3_t{0,0,4});
                    this->matrices.view.rotate(fvec3_t{1,0,0}, 80.0f);
                    this->matrices.view.translate(fvec3_t{-1.0f * glm::clamp(plinv.x / (100.0f * (SPEED_SCALE / 0.2f)), -1.0f, 1.0f), -glm::clamp(plinv.z/(75.0f * (SPEED_SCALE / 0.2f)), -1.0f, 1.0f), this->camdist});
                    plspeed = glm::length(plinv);
                }
                this->matrices.projection.perspectiveX(cam_fov, this->R_WindowAspect(), 0.1f, 30000.0f);

                this->main_scene->draw(mld.uptime);

                //if (this->draw_touch_controls){this->tcsys->redraw_controls();}
                this->matrices.model[-1] = fmat44_t(1);
                //this->R_DrawTestTriangle();
                this->R_ClearBuffers(false,true);
                //this->R_SetCapEnabled(rendercap::depth_test, false);
                char speed_str[129] = {}; char32_t speed_str_u[129] = {};
                sprintf(speed_str, "%%cw%4iK", int(round(plspeed/SPEED_SCALE)));
                std::copy_n(speed_str, strlen(speed_str), speed_str_u);
                fscalar_t aspect = this->R_WindowAspect();
                this->matrices.projection[-1] = glm::orthoRH(-aspect, aspect, -1.0f, 1.0f, 0.1f, 100.0f);
                this->matrices.view.loadIdentity(); this->matrices.view.translate(fvec3_t{0,0,1});
                //this->matrices.model.loadIdentity();
                /*
                this->matrices.model.translate(fvec3_t{0.35f, -0.15f, -1.0f});
                this->matrices.model.scale(fvec3_t(0.1f));
                */
                this->matrices.model[-1] = fmat44_t(trs_t(
                    fvec3_t{0.75f, -0.8f, -1.03f},
                    fmat33_t(1),
                    fvec3_t{0.1f, 0.1f, 1.0f}
                ));
                //this->R_SetCapEnabled(rendercap::blending, true);
                this->R_DrawBitmapText(uid_t(this->fzgx_speedo_font[0]), speed_str_u);
                //this->R_SetCapEnabled(rendercap::depth_test, true);
                //this->R_SetCapEnabled(rendercap::blending, false);
                this->R_RedrawConsole();
                //std::cout << "GONNA SWAP BUFFERS\n";
                this->R_SwapBuffers();
                //std::cout << "SWAPPED BUFFERS\n";
            }
			return true;
        }
};

int main(int argc, const char* argv[]) {
    global_startup();
    SpaceShooterMain::params_t args;
    ArgumentParser<char> ap; ap.set_argsyntax(argprefix::dash, argsyntax::name_equal_value).load_tokens(argv+1, argv+argc);
    for (parsedarg_t<char> parg; ap.adv(parg);){
        switch (parg.prefix) {
            case argprefix::dash: {
                if (parg.name == "renderscale"){args.renderscale = std::stoi(parg.value);}
                else if (parg.name == "width"){args.width = std::stoi(parg.value);}
                else if (parg.name == "height"){args.height = std::stoi(parg.value);}
                else if (parg.name == "basedir"){args.basedir = parg.value;}
                else if (parg.name == "map"){args.mapname = parg.value;}
                else if (parg.name == "machine"){args.machine = "/machines/" + parg.value + ".mac";}
                else if (parg.name == "mod"){args.mods.push_back(parg.value);}
                break;
            }
            case argprefix::double_dash: {
                if (parg.name == "fullscreen"){args.fullscreen = true;}
                else if (parg.name == "swmode"){args.swmode = true;}
                break;
            }
            default: ;
        }
    }
    auto* i1 = new_engine_instance<SpaceShooterMain>(&args);
    global_mainloop();
    global_shutdown();
    return 0;
}
/*
#if (DJUTIL_OSTYPE == 1)

int wmain(int argc, const wchar_t* argv[]){
    std::vector<const char*> argv8; argv8.resize(argc);
    for (int i = 0; i < argc; i++){
        std::string arg8_1 = djutil::ezstr::utf16_to_utf8((char16_t*)argv[i]);
        char* arg8 = new char[arg8_1.size()+1];
        memcpy(arg8, arg8_1.c_str(), arg8_1.size()+1);
        argv8[i] = arg8;
    }
    return main(argc, argv8.data());
}
#endif
*/
