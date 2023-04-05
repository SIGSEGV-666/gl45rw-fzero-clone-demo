
#include <iostream>
#include <string>
#include <valarray>
#include <array>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <functional>


#include "glm/gtc/matrix_integer.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/noise.hpp"
#include "glm/gtc/packing.hpp"
#include "glm/gtc/color_space.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/io.hpp"
#include "glm/gtx/normal.hpp"

#include "glm/ext/scalar_common.hpp"
#include "glm/ext/vector_common.hpp"
#include "glm/gtc/integer.hpp"
#include "glm/ext/matrix_projection.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/intersect.hpp"
#include "glm/gtx/fast_trigonometry.hpp"
#include "glm/gtx/wrap.hpp"

#include "glm/gtx/extended_min_max.hpp"

namespace sw3d3 {

    using ra16_t = glm::vec<2,uint8_t>;
    using rgb24_t = glm::vec<3,uint8_t>;
    using rgba32_t = glm::vec<4,uint8_t>;

    using glm::ivec2;

    using glm::vec2;
    using glm::vec3;
    using glm::vec4;

    using glm::mat2;
    using glm::mat3;
    using glm::mat4;

    static const float INV255F = 1/255.0f;



    namespace _tri_bisect {
        static const size_t _TRI_EDGES[3][2] = {
            {0,1},
            {1,2},
            {2,0}
        };
        static const size_t _TRI_BACK_IDXS[3][2] = {
            {1, 2},
            {0, 2},
            {0, 1}
        };

        struct clipvertex_t {
            glm::vec3 barycentric_weights, co;
        };

        struct cliptriangle_t {
            clipvertex_t v0, v1, v2;
        };


        glm::vec3 barycentric_pt_tri3d(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
            const float invarea2 = 1.0f/glm::length(glm::cross(b-a, c-a));
            glm::vec3 bc = {
                glm::length(glm::cross(p-b, p-c))*invarea2,
                glm::length(glm::cross(p-c, p-a))*invarea2,
                0.0f
            };
            bc.z = 1.0f - bc.x - bc.y;
            return bc;
        }

        glm::vec3 _tri_edgeclip(const glm::vec3& plane_co, const glm::vec3& plane_normal, const glm::vec3& a, const glm::vec3& b){
            float idist = 0;
            const glm::vec3 dir = glm::normalize(b-a);
            if (glm::intersectRayPlane(a, dir, plane_co, plane_normal, idist)){
                return a+(dir*idist);
            }
            return b;
        }

        void calc_front_clipvertices_1front(
            cliptriangle_t& cliptri,
            const glm::vec3 tedges[3][2],
            const glm::vec3& plane_co,
            const glm::vec3& plane_normal,
            const size_t& frontvertex
        )
        {
            const size_t* backvertices = _TRI_BACK_IDXS[frontvertex];
            clipvertex_t* cverts = &cliptri.v0;
            for (int i = 0; i < 2; i++) {
                const size_t bvi = backvertices[i];
                const glm::vec3 fvco = tedges[frontvertex][0];
                const glm::vec3 bvco = tedges[bvi][0];
                const glm::vec3 clipb = _tri_edgeclip(plane_co, plane_normal, fvco, bvco);
                cverts[frontvertex] = clipvertex_t{.co=fvco};
                cverts[bvi] = clipvertex_t{.co=clipb};
            }
        }
        void calc_front_clipvertices_2front(
            cliptriangle_t* cliptris,
            const glm::vec3 tedges[3][2],
            const glm::vec3& plane_co,
            const glm::vec3& plane_normal,
            const size_t& frontvertex1,
            const size_t& frontvertex2
        )
        {
            const size_t frontverts[2] = {frontvertex1, frontvertex2};
            size_t backvertex;
            for (backvertex=0; backvertex < 3; backvertex++){
                if (backvertex != frontvertex1 && backvertex != frontvertex2){break;}
            }

            const glm::vec3
                a = tedges[frontvertex1][0],
                b = tedges[frontvertex2][0],
                c = tedges[backvertex][0],
                aprime = _tri_edgeclip(plane_co, plane_normal, a, c),
                bprime = _tri_edgeclip(plane_co, plane_normal, b, c)
            ;

            cliptris[0] = cliptriangle_t{
                .v0=clipvertex_t{.co=a},
                .v1=clipvertex_t{.co=aprime},
                .v2=clipvertex_t{.co=bprime}
            };
            cliptris[1] = cliptriangle_t{
                .v0=clipvertex_t{.co=a},
                .v1=clipvertex_t{.co=bprime},
                .v2=clipvertex_t{.co=b}
            };
        }

        bool clip_tri_plane(
            size_t& numback, cliptriangle_t* backtris,
            size_t& numfront, cliptriangle_t* fronttris,
            const glm::vec3& plane_co, const glm::vec3& plane_normal,
            const glm::vec3& tri_v0,
            const glm::vec3& tri_v1,
            const glm::vec3& tri_v2
        ){
            const glm::vec3 vvectors[3] = {
                glm::normalize(tri_v0-plane_co),
                glm::normalize(tri_v1-plane_co),
                glm::normalize(tri_v2-plane_co)
            };

            const glm::vec3 tverts[3] = {
                tri_v0,
                tri_v1,
                tri_v2
            };

            const glm::vec3 tedges[3][2] = {
                {tverts[_TRI_EDGES[0][0]], tverts[_TRI_EDGES[0][1]]},
                {tverts[_TRI_EDGES[1][0]], tverts[_TRI_EDGES[1][1]]},
                {tverts[_TRI_EDGES[2][0]], tverts[_TRI_EDGES[2][1]]}
            };

            size_t frontverts[3] = {}, backverts[3] = {};
            size_t numfrontverts = 0, numbackverts = 0;

            for (size_t i = 0; i < 3; i++){
                bool isfront = (glm::dot(vvectors[i], plane_normal) >= 0);
                if (isfront){frontverts[numfrontverts++] = i;}
                else {backverts[numbackverts++] = i;}
            }
            numfront = numback = 0;
            if (numfrontverts == 3) {
                numfront = 1;
                fronttris[0] = cliptriangle_t{
                    .v0=clipvertex_t{.barycentric_weights=barycentric_pt_tri3d(tverts[0], tri_v0, tri_v1, tri_v2), .co=tverts[0]},
                    .v1=clipvertex_t{.barycentric_weights=barycentric_pt_tri3d(tverts[1], tri_v0, tri_v1, tri_v2), .co=tverts[1]},
                    .v2=clipvertex_t{.barycentric_weights=barycentric_pt_tri3d(tverts[2], tri_v0, tri_v1, tri_v2), .co=tverts[2]}
                };
                return false;
            }
            else if (numbackverts == 3) {
                numback = 1;
                backtris[0] = cliptriangle_t{
                    .v0=clipvertex_t{.barycentric_weights=barycentric_pt_tri3d(tverts[0], tri_v0, tri_v1, tri_v2), .co=tverts[0]},
                    .v1=clipvertex_t{.barycentric_weights=barycentric_pt_tri3d(tverts[1], tri_v0, tri_v1, tri_v2), .co=tverts[1]},
                    .v2=clipvertex_t{.barycentric_weights=barycentric_pt_tri3d(tverts[2], tri_v0, tri_v1, tri_v2), .co=tverts[2]}
                };
                return false;
            }

            else if (numfrontverts == 1) {
                numfront = 1;
                numback = 2;
                calc_front_clipvertices_1front(fronttris[0], tedges, plane_co, plane_normal, frontverts[0]);
                calc_front_clipvertices_2front(backtris, tedges, plane_co, -plane_normal, backverts[0], backverts[1]);
            }
            else if (numbackverts == 1) {
                numback = 1;
                numfront = 2;
                calc_front_clipvertices_1front(backtris[0], tedges, plane_co, -plane_normal, backverts[0]);
                calc_front_clipvertices_2front(fronttris, tedges, plane_co, plane_normal, frontverts[0], frontverts[1]);
            }

            for (size_t ctn=0; ctn < 2; ctn++){
                for (size_t i = 0; i < 3; i++){
                    if (ctn < numback){
                        clipvertex_t* btverts = &(backtris[ctn].v0);
                        btverts[i].barycentric_weights = barycentric_pt_tri3d(btverts[i].co, tri_v0, tri_v1, tri_v2);
                    }
                    if (ctn < numfront){
                        clipvertex_t* ftverts = &(fronttris[ctn].v0);
                        ftverts[i].barycentric_weights = barycentric_pt_tri3d(ftverts[i].co, tri_v0, tri_v1, tri_v2);
                    }
                }
            }
            return true;
        }
    }

    inline ivec2 iv2min3(const ivec2& a, const ivec2& b, const ivec2& c) {
        return ivec2{
            glm::min(glm::min(a.x, b.x), c.x),
            glm::min(glm::min(a.y, b.y), c.y)
        };
    }
    inline ivec2 iv2max3(const ivec2& a, const ivec2& b, const ivec2& c) {
        return ivec2{
            glm::max(glm::max(a.x, b.x), c.x),
            glm::max(glm::max(a.y, b.y), c.y)
        };
    }
    bool scanline_lineseg2D(float& factor, float& ix, const float& y, const float& x0, const float& y0, const float& x1, const float& y1) {
        const float
            ldx = x1 - x0,
            ldy = y1 - y0,
            ydelta = y - y0
        ;
        factor = ydelta/ldy;
        ix = x0 + (ldx * factor);
        return (factor >= 0.0f && factor <= 1.0f);
    }
    struct isect_t {
        float factor, ix;
        int edge;
    };
    struct drawspan_t {
        int numisects = 0;
        isect_t isects[2] = {};
        bool _ok2add = false;
        float x0, x1;
        float edge0fac, edge1fac;
        int edge0, edge1;
        int x0i, x1i;

        bool try_isect(const int& edge, const float& y, const float& x0, const float& y0, const float& x1, const float& y1) {
            if (this->numisects >= 2){return true;}
            isect_t cand = {.edge=edge};
            if (scanline_lineseg2D(cand.factor, cand.ix, y, x0,y0, x1,y1)){
                if (this->numisects > 0){
                    if (cand.ix == this->isects[0].ix){
                        return false;
                    }
                }
                this->isects[this->numisects++] = cand;
                if (this->numisects == 2){this->_ok2add = true; return true;}
            }
            return false;
        }
        bool calc_span(const int& xmin, const int& ymin, const int& xmax, const int& ymax) {
            if (!this->_ok2add){
                return false;
            }
            auto& is0 = this->isects[0];
            auto& is1 = this->isects[1];

            this->x0 = std::min(is0.ix, is1.ix);
            this->x1 = std::max(is0.ix, is1.ix);

            this->x0i = std::max(xmin, int(this->x0));
            this->x1i = std::min(xmax, int(this->x1));

            auto& isleft = (is0.ix < is1.ix ? is0 : is1);
            auto& isright = (is0.ix < is1.ix ? is1 : is0);

            this->edge0 = isleft.edge;
            this->edge1 = isright.edge;

            this->edge0fac = isleft.factor;
            this->edge1fac = isright.factor;

            return true;
        }
    };

    const std::array<int, 2> _EDGEVERTINDICES[3] = {
        {0,1},
        {1,2},
        {2,0}
    };

    template <class ET>
    class MemoryBuffer {
        private:
            ET* _buffer = nullptr;
            size_t _bufnelems = 0;
            bool _owns_buffer = false;
        public:
            ET* data() {return this->_buffer;}
            inline size_t size() {return this->_bufnelems;}
            inline size_t nbytes() {return this->_bufnelems * sizeof(ET);}
            inline size_t esizeof() {return sizeof(ET);}

            void reset() {
                if (this->_owns_buffer && this->_buffer != nullptr){
                    delete[] this->_buffer;
                }
                this->_owns_buffer = false;
                this->_buffer = nullptr;
                this->_bufnelems = 0;
            }
            void resize(const size_t n, const ET fv=ET()){
                this->reset();
                if (n == 0){return;}
                this->_owns_buffer = true;
                this->_buffer = new ET[n];
                this->_bufnelems = n;
                std::fill_n(this->_buffer, n, fv);
            }
            void setsharedbuf(const size_t n, const ET* b){
                if (n == 0 || b == nullptr){return;}
                this->reset();
                this->_bufnelems = n;
                this->_buffer = (ET*)b;
                this->_owns_buffer = false;
            }

            inline ET& operator[](const size_t idx){return this->_buffer[idx];}
            inline ET& front() {return this->_buffer[0];}
            inline ET& back() {return this->_buffer[this->_bufnelems - 1];}

            inline ET* pfront() {return &this->front();}
            inline ET* pback() {return &this->back();}

            inline operator ET*() {return this->_buffer;}

            inline ET* begin() {return this->pfront();}
            inline ET* end() {return this->pback()+1;}

            MemoryBuffer() {}
            MemoryBuffer(const size_t n, const ET* b) {this->setsharedbuf(n,b);}
            ~MemoryBuffer() {this->reset();}
    };

    struct pixelrowcoord_t {
        ivec2 xy;
        const ivec2& operator*() const {return this->xy;}
        ivec2& operator*() {return this->xy;}
        bool operator==(const pixelrowcoord_t& other) const {return (this->xy.x == other.xy.x && this->xy.y == other.xy.y);}
        bool operator!=(const pixelrowcoord_t& other) const {return (this->xy.x != other.xy.x || this->xy.y != other.xy.y);}
        pixelrowcoord_t operator++(int){
            const auto old = *this;
            this->xy.x++;
            return old;
        }
        pixelrowcoord_t() {}
        pixelrowcoord_t(const int x, const int y) : xy{x,y} {}
        pixelrowcoord_t(const pixelrowcoord_t& o) : xy(o.xy) {}
        ~pixelrowcoord_t() {}
    };

    class TestContext {
        public:
            enum cullface_e : int {
                disabled = 0,
                back = 1,
                front = 2
            };
            enum depthfunc_e : int {
                less = 0,
                less_equal = 1,
                greater = 2,
                greater_equal = 3,
                equal = 4,
                not_equal = 5,
                always = 6,
                never = 7
            };
            static bool _z_compare(const depthfunc_e& f, const float& cand, const float& cur) {
                switch (f) {
                    case depthfunc_e::less: return (cand < cur);
                    case depthfunc_e::less_equal: return (cand <= cur);
                    case depthfunc_e::greater: return (cand > cur);
                    case depthfunc_e::greater_equal: return (cand >= cur);
                    case depthfunc_e::equal: return (cand == cur);
                    case depthfunc_e::not_equal: return (cand != cur);
                    case depthfunc_e::always: return true;
                    case depthfunc_e::never:
                    default: return false;
                }
            }
            enum textarget_e : int {
                target2d = 0,

                cube_pos_x = 1,
                cube_neg_x = 2,

                cube_pos_y = 3,
                cube_neg_y = 4,

                cube_pos_z = 5,
                cube_neg_z = 6
            };
            enum texelfmt_e : int {
                none = 0,

                r8 = 1,
                ra8 = 2,
                rgb8 = 3,
                rgba8 = 4,

                depth32f = 5
            };

            enum texwrap_e : int {
                repeat = 0,
                clamp_to_edge = 1,
                mirrored_repeat = 2
            };

            enum texfilter_e : int {
                nearest = 0,
                linear = 1
            };

            static inline size_t texelfmt_nbytes(const texelfmt_e& tfmt) {
                switch (tfmt){
                    case texelfmt_e::r8: return 1;
                    case texelfmt_e::ra8: return 2;
                    case texelfmt_e::rgb8: return 3;
                    case texelfmt_e::rgba8:
                    case texelfmt_e::depth32f: return 4;
                    default: return 0;
                }
            }


            enum texcoords_e : int {
                uv0 = 0,
                uv1 = 1,
                uv2 = 2,
                uv3 = 3,
                uv4 = 4,
                uv5 = 5,
                uv6 = 6,
                uv7 = 7
            };
            struct teximg8_t {
                unsigned int idx;
                unsigned int w, h;
                std::valarray<rgba32_t> p;
            };

            struct texture_t {
                unsigned int idx;
                struct _target {
                    int w, h;
                    texelfmt_e fmt;

                    texwrap_e wrap_s = texwrap_e::repeat;
                    texwrap_e wrap_t = texwrap_e::repeat;

                    texfilter_e filter = texfilter_e::nearest;

                    bool flip_u_axis = false, flip_v_axis = false;

                    MemoryBuffer<uint8_t> _texels_u8;

                    void _gettexel4idx(vec4& outtexel, const size_t& idx) {
                        static const void* _jmps[] = {
                            &&_fmt_none,
                            &&_fmt_r8,
                            &&_fmt_ra8,
                            &&_fmt_rgb8,
                            &&_fmt_rgba8,
                            &&_fmt_depth32f
                        };

                        goto *_jmps[int(this->fmt)];
                        #define _rig(_tn) (reinterpret_cast<_tn*>(this->_texels_u8.pfront())[idx])
                        _fmt_none: return;
                        _fmt_r8: outtexel = vec4{this->_texels_u8[idx] * INV255F, 0.0f, 0.0f, 1.0f}; return;
                        _fmt_ra8: {
                            auto& t = _rig(ra16_t);
                            outtexel = vec4{t.x*INV255F, 0.0f, 0.0f, t.y*INV255F};
                            return;
                        }
                        _fmt_rgb8: {
                            outtexel = vec4(vec3(_rig(rgb24_t))*INV255F, 1.0f);
                            return;
                        }
                        _fmt_rgba8: {
                            outtexel = vec4(_rig(rgba32_t))*INV255F;
                            return;
                        }
                        _fmt_depth32f: {
                            outtexel.x = outtexel.y = outtexel.z = outtexel.w = _rig(float);
                            return;
                        }
                        #undef _rig

                    }
                    void _settexel4idx(const size_t& idx, const vec4& intexel) {
                        static const void* _jmps[] = {
                            &&_fmt_none,
                            &&_fmt_r8,
                            &&_fmt_ra8,
                            &&_fmt_rgb8,
                            &&_fmt_rgba8,
                            &&_fmt_depth32f
                        };

                        goto *_jmps[int(this->fmt)];
                        #define _rig(_tn) (reinterpret_cast<_tn*>(this->_texels_u8.pfront())[idx])
                        _fmt_none: return;
                        _fmt_r8: this->_texels_u8[idx] = glm::clamp(intexel.r, 0.0f, 1.0f)*255; return;
                        _fmt_ra8: {
                            auto& t = _rig(ra16_t);
                            t.x = glm::clamp(intexel.r, 0.0f, 1.0f)*255;
                            t.y = glm::clamp(intexel.a, 0.0f, 1.0f)*255;
                            return;
                        }
                        _fmt_rgb8: {
                            auto& t = _rig(rgb24_t);
                            t = rgb24_t(glm::clamp(vec3(intexel), vec3(0), vec3(1))*255.0f);
                            return;
                        }
                        _fmt_rgba8: {
                            _rig(rgba32_t) = rgba32_t(glm::clamp(intexel, vec4(0), vec4(1)) * 255.0f);
                            return;
                        }
                        _fmt_depth32f: {
                            _rig(float) = intexel.r;
                            return;
                        }
                        #undef _rig

                    }
                    inline size_t _idx4xy(const ivec2& xy) {
                        return ((xy.x < 0 || xy.y < 0 || xy.x >= this->w || xy.y >= this->h) ? size_t(-1) : size_t((this->w * xy.y)+xy.x));
                    }
                    float _texwrapaxis(const int& axis, const float& a) {
                        static const void* _jmps[] = {
                           &&_w_repeat,
                           &&_w_clamp_to_edge,
                           &&_w_mirrored_repeat
                        };
                        goto *_jmps[int((axis == 0 ? this->wrap_s : this->wrap_t))];

                        _w_repeat: return glm::repeat(a);
                        _w_clamp_to_edge: return glm::clamp(a);
                        _w_mirrored_repeat: return glm::mirrorRepeat(a);
                        return 0.0f;
                    }


                };
                std::array<_target, 7> targets = {};

                uint8_t* _gettexelptr_for_xy(const textarget_e& tgname, const ivec2& xy){
                    auto& tg = this->targets[int(tgname)];
                    if (xy.x < 0 || xy.y < 0 || xy.x >= tg.w || xy.y >= tg.h){return nullptr;}
                    else {
                        return &(tg._texels_u8[((tg.w * xy.y) + xy.x)*texelfmt_nbytes(tg.fmt)]);
                    }
                }

                static inline size_t _idx4xy(const ivec2& wh, const ivec2& xy) {
                    return ((xy.x < 0 || xy.y < 0 || xy.x >= wh.x || xy.y >= wh.y) ? size_t(-1) : size_t((wh.x * xy.y)+xy.x));
                }

                ivec2 _uv2xy(const int w, const int h, const bool flip_u, const bool flip_v, const vec2& uv) {
                    const float w1 = w-1, h1 = h-1;
                    return ivec2{
                        int(glm::round(flip_u ? w1-(w1*uv.x) : w1*uv.x)),
                        int(glm::round(flip_v ? h1-(h1*uv.y) : h1*uv.y))
                    };
                }

                static inline float _texwrapaxis(const texwrap_e& w, const float& a) {
                    switch (w) {
                        case texwrap_e::clamp_to_edge: return glm::clamp(a);
                        case texwrap_e::mirrored_repeat: return glm::mirrorRepeat(a);
                        default: return glm::repeat(a);
                    }
                }

                static inline vec2 _texwrapst(const texwrap_e& ws, const texwrap_e& wt, const vec2& st){
                    return vec2{
                        _texwrapaxis(ws, st.x),
                        _texwrapaxis(wt, st.y)
                    };
                }

                template <class OIT, class IIT>
                void get_texels_for_xys(const textarget_e& tgname, OIT ostart, IIT istart, IIT iend) {
                    auto& tg = this->targets[int(tgname)];
                    const size_t fmtnb = texelfmt_nbytes(tg.fmt);
                    const uint8_t* texels_r8 = tg._texels_u8.pfront();
                    auto texels_ra16 = reinterpret_cast<const ra16_t*>(texels_r8);
                    auto texels_rgb24 = reinterpret_cast<const rgb24_t*>(texels_r8);
                    auto texels_rgba32 = reinterpret_cast<const rgba32_t*>(texels_r8);
                    auto texels_depth32f = reinterpret_cast<const float*>(texels_r8);
                    #define _itrline() oitr = ostart; for (IIT iitr = istart; iitr != iend; iitr++, oitr++)
                    #define _itrhdr() vec4& outcolor = *oitr; const ivec2& xy = *iitr; const size_t pxidx = TestContext::texture_t::_idx4xy(tgwh, xy);
                    OIT oitr = ostart;
                    const ivec2 tgwh = {tg.w, tg.h};
                    const size_t nidx = size_t(-1);
                    const vec4 zero(0);
                    const float inv255f = 1/255.0f;
                    switch (tg.fmt) {
                        case texelfmt_e::r8: {
                            _itrline() {_itrhdr();
                                outcolor = (pxidx == nidx ? zero : vec4{inv255f * float(texels_r8[pxidx]), 0.0f, 0.0f, 1.0f});
                            }
                            break;
                        }
                        case texelfmt_e::ra8: {
                            _itrline() {_itrhdr();
                                if (pxidx == nidx){outcolor = zero;}
                                else {
                                    const auto& _t = texels_ra16[pxidx];
                                    outcolor = vec4{
                                        float(_t.x) * inv255f,
                                        0.0f,
                                        0.0f,
                                        float(_t.y) * inv255f
                                    };
                                }
                            }
                            break;
                        }
                        case texelfmt_e::rgb8: {
                            _itrline() {_itrhdr();
                                outcolor = (pxidx == nidx ? zero : vec4(vec3(texels_rgb24[pxidx]) * inv255f, 1));
                            }
                            break;
                        }
                        case texelfmt_e::rgba8: {
                            _itrline() {_itrhdr();
                                outcolor = (pxidx == nidx ? zero : vec4(texels_rgba32[pxidx])*inv255f);
                            }
                            break;
                        }
                        case texelfmt_e::depth32f: {
                            _itrline() {_itrhdr();
                                outcolor = (pxidx == nidx ? vec4(-1) : vec4(texels_depth32f[pxidx]));
                            }
                            break;
                        }
                        default: ;
                    }
                    #undef _itrline
                    #undef _itrhdr
                }
                void clear_texels(const textarget_e& tgname, const vec4& color){
                    auto& tg = this->targets[int(tgname)];
                    const size_t fmtnb = texelfmt_nbytes(tg.fmt);
                    const size_t npx = tg.w * tg.h;

                    uint8_t* texels_r8 = tg._texels_u8.pfront();
                    auto texels_ra16 = reinterpret_cast<ra16_t*>(texels_r8);
                    auto texels_rgb24 = reinterpret_cast<rgb24_t*>(texels_r8);
                    auto texels_rgba32 = reinterpret_cast<rgba32_t*>(texels_r8);
                    auto texels_depth32f = reinterpret_cast<float*>(texels_r8);

                    switch (tg.fmt) {
                        case texelfmt_e::r8: {
                            std::fill_n(texels_r8, npx, uint8_t(glm::clamp(color.r * 255, 0.0f, 255.0f)));
                            break;
                        }
                        case texelfmt_e::ra8: {
                            std::fill_n(texels_ra16, npx, ra16_t(glm::clamp(vec2{color.r, color.a}*255.0f, vec2(0), vec2(255))));
                            break;
                        }
                        case texelfmt_e::rgb8: {
                            std::fill_n(texels_rgb24, npx, rgb24_t(glm::clamp(vec3(color)*255.0f, vec3(0), vec3(255))));
                            break;
                        }
                        case texelfmt_e::rgba8: {
                            std::fill_n(texels_rgba32, npx, rgba32_t(glm::clamp(color*255.0f, vec4(0), vec4(255))));
                            break;
                        }
                        case texelfmt_e::depth32f: {
                            std::fill_n(texels_depth32f, npx, color.r);
                            break;
                        }
                        default: ;
                    }
                }
                template <class XYIT, class COLIT>
                void set_texels_for_xys(const textarget_e& tgname, XYIT xystart, XYIT xyend, COLIT colstart) {
                    auto& tg = this->targets[int(tgname)];
                    const size_t fmtnb = texelfmt_nbytes(tg.fmt);
                    uint8_t* texels_r8 = tg._texels_u8.pfront();
                    auto texels_ra16 = reinterpret_cast<ra16_t*>(texels_r8);
                    auto texels_rgb24 = reinterpret_cast<rgb24_t*>(texels_r8);
                    auto texels_rgba32 = reinterpret_cast<rgba32_t*>(texels_r8);
                    auto texels_depth32f = reinterpret_cast<float*>(texels_r8);
                    COLIT colitr = colstart;
                    const ivec2 tgwh = {tg.w, tg.h};
                    const size_t nidx = size_t(-1);
                    #define _itrline() colitr = colstart; for (XYIT xyitr = xystart; xyitr != xyend; xyitr++, colitr++)
                    #define _itrhdr() const vec4& newcolor = *colitr; const ivec2& xy = *xyitr; const size_t pxidx = TestContext::texture_t::_idx4xy(tgwh, xy);

                    switch (tg.fmt) {
                        case texelfmt_e::r8: {
                            _itrline() {_itrhdr();
                                if (pxidx != nidx){
                                    texels_r8[pxidx] = uint8_t(glm::clamp(255.0f * newcolor.r, 0.0f, 255.0f));
                                }
                            }
                            break;
                        }
                        case texelfmt_e::ra8: {
                            _itrline() {_itrhdr();
                                if (pxidx != nidx){
                                    texels_ra16[pxidx] = ra16_t(glm::clamp(vec2{newcolor.r, newcolor.a} * 255.0f, vec2(0.0f), vec2(255.0f)));
                                }
                            }
                            break;
                        }
                        case texelfmt_e::rgb8: {
                            _itrline() {_itrhdr();
                                if (pxidx != nidx){
                                    texels_rgb24[pxidx] = rgb24_t(glm::clamp(vec3(newcolor) * 255.0f, vec3(0), vec3(255)));
                                }
                            }
                            break;
                        }
                        case texelfmt_e::rgba8: {
                            _itrline() {_itrhdr();
                                if (pxidx != nidx){
                                    texels_rgba32[pxidx] = rgba32_t(glm::clamp(newcolor * 255.0f, vec4(0), vec4(255)));
                                }
                            }
                            break;
                        }

                        case texelfmt_e::depth32f: {
                            _itrline() {_itrhdr();
                                if (pxidx != nidx){
                                    texels_depth32f[pxidx] = newcolor.r;
                                }
                            }
                            break;
                        }

                        default: ;
                    }

                    #undef _itrline
                    #undef _itrhdr
                }
                vec4 _sampleTarget2D(const textarget_e& tgname, const vec2& st) {
                    auto& tg = this->targets[int(tgname)];
                    const auto ws = tg.wrap_s;
                    const auto wt = tg.wrap_t;
                    /*
                    vec4 outcolor;
                    switch (tg.filter) {
                        case texfilter_e::linear: {
                            const vec2 maxcof = vec2{tg.w-1, tg.h-1}, pxst = vec2(1) / maxcof;
                            const vec2 baseuv = {
                                _texwrapaxis(tg.wrap_s, st.x),
                                _texwrapaxis(tg.wrap_s, st.y)
                            };
                            const vec2 bluv = vec2(this->_uv2xy(tg.w, tg.h, tg.flip_u_axis, tg.flip_v_axis, baseuv)) * maxcof, truv = bluv + pxst;
                            const vec2 factor = (baseuv - bluv) * maxcof;
                            const std::array<ivec2, 4> corner_xys = {
                                ivec2(maxcof * _texwrapst(ws,wt,bluv)), ivec2(maxcof * _texwrapst(ws,wt,vec2{truv.x, bluv.y})),
                                ivec2(maxcof * _texwrapst(ws,wt,vec2{bluv.x, truv.y})), ivec2(maxcof * _texwrapst(ws,wt,truv))
                            };
                            std::array<vec4, 4> corner_colors = {};
                            this->get_texels_for_xys(tgname, corner_colors.begin(), corner_xys.begin(), corner_xys.end());
                            outcolor = glm::mix(
                                glm::mix(corner_colors[0], corner_colors[1], factor.x),
                                glm::mix(corner_colors[2], corner_colors[3], factor.x),
                                factor.y
                            );
                            break;
                        }
                        default: {
                            //const ivec2 xy = ivec2(glm::round(vec2{tg.w-1, tg.h-1} * _texwrapst(ws,wt,st)));
                            const ivec2 xy = this->_uv2xy(tg.w, tg.h, tg.flip_u_axis, tg.flip_v_axis, _texwrapst(ws,wt,st));
                            this->get_texels_for_xys(tgname, &outcolor, &xy, (&xy)+1);
                        }
                    }
                    */
                    size_t idx = _idx4xy(ivec2{tg.w, tg.h}, _uv2xy(tg.w, tg.h, tg.flip_u_axis, tg.flip_v_axis, _texwrapst(ws,wt,st)));
                    static const void* jmps[] = {
                        &&_fmt_none,
                        &&_fmt_r8,
                        &&_fmt_ra8,
                        &&_fmt_rgb8,
                        &&_fmt_rgba8,
                        &&_fmt_depth32f
                    };
                    ra16_t texel_ra16 = {};
                    goto *jmps[int(tg.fmt)];
                    _fmt_r8: return vec4{tg._texels_u8[idx]*INV255F,0.0f,0.0f,1.0f};
                    _fmt_ra8:
                        texel_ra16 = reinterpret_cast<ra16_t*>(tg._texels_u8.pfront())[idx];
                        return vec4{texel_ra16.x*INV255F, 0.0f, 0.0f, texel_ra16.y*INV255F};
                    _fmt_rgb8:
                        return vec4(vec3(reinterpret_cast<rgb24_t*>(tg._texels_u8.pfront())[idx])*INV255F, 1);
                    _fmt_rgba8:
                        return vec4(reinterpret_cast<rgba32_t*>(tg._texels_u8.pfront())[idx])*INV255F;
                    _fmt_depth32f:
                        return vec4(reinterpret_cast<float*>(tg._texels_u8.pfront())[idx]);
                    _fmt_none:
                        return vec4(0);
                }

                vec4 texture2D(const vec2& st) {
                    return this->_sampleTarget2D(textarget_e::target2d, st);
                }

                vec4 textureEquirect(const vec3& vec) {
                    const float vl = 1/glm::length(vec);
                    return this->_sampleTarget2D(textarget_e::target2d, vec2{
                            (0.5f)+(atan2f((vec.x*vl), (vec.y*vl))/(2.0f * 3.14159f)),
                            (0.5f)-(asinf(-vec.z*vl)/3.14519f)
                    });

                }

                bool texImage2D(const textarget_e& targetname, const int w, const int h, const texelfmt_e& fmt, const void* pixels, const bool copy=true){
                    if (w <= 0 || h <= 0){return false;}
                    if (fmt == texelfmt_e::none){return false;}
                    auto& tg = this->targets[targetname];
                    auto& tb = tg._texels_u8;
                    tb.reset();
                    const size_t nnpx = w * h, ntnb = texelfmt_nbytes(fmt), nsize = nnpx * ntnb;
                    tg.w = w;
                    tg.h = h;
                    tg.fmt = fmt;
                    if (copy || pixels == nullptr){
                        tb.resize(nsize);
                        if (pixels != nullptr){std::copy_n((const uint8_t*)pixels, nsize, tb.pfront());}
                    }
                    else {
                        tb.setsharedbuf(nsize, (const uint8_t*)pixels);
                    }
                    return true;
                }

            };

            enum struct fbo_status_e : int {
                ok = 0,

                bad_color_attachment_target = 1,
                bad_depth_attachment_target = 2,

                color_mismatched_width = 3,
                color_mismatched_height = 4,
                bad_color_texid = 5,

                depth_mismatched_width = 6,
                depth_mismatched_height = 7,
                bad_depth_texid = 8,

                invalid_width = 9,
                invalid_height = 10
            };

            enum struct fbo_attachment_e : int {
                color = 0,
                depth = 1
            };
            struct fbo_t {
                TestContext* ctx = nullptr;
                unsigned int idx = 0;
                int w, h;

                struct _attachment {
                    textarget_e target = textarget_e::target2d;
                    unsigned int texture = 0;
                };

                std::array<_attachment, 2> attachments = {};


                fbo_status_e validate() {
                    if (this->w <= 0){return fbo_status_e::invalid_width;}
                    if (this->h <= 0){return fbo_status_e::invalid_height;}

                    auto& color_attachment = this->attachments[0];
                    auto& depth_attachment = this->attachments[1];

                    if (color_attachment.texture != 0){
                        if (!this->ctx->isTexture(color_attachment.texture)){return fbo_status_e::bad_color_texid;}
                        auto* coltex = this->ctx->getTexture(color_attachment.texture);
                        if (int(color_attachment.target) < 0 || int(color_attachment.target) >= coltex->targets.size()){return fbo_status_e::bad_color_attachment_target;}
                        auto& coltg = coltex->targets[int(color_attachment.target)];
                        if (coltg.w != this->w){return fbo_status_e::color_mismatched_width;}
                        if (coltg.h != this->h){return fbo_status_e::color_mismatched_height;}
                    }
                    if (depth_attachment.texture != 0){
                        if (!this->ctx->isTexture(depth_attachment.texture)){return fbo_status_e::bad_depth_texid;}
                        auto* depthtex = this->ctx->getTexture(depth_attachment.texture);
                        if (int(depth_attachment.target) < 0 || int(depth_attachment.target) >= depthtex->targets.size()){return fbo_status_e::bad_depth_attachment_target;}
                        auto& depthtg = depthtex->targets[int(depth_attachment.target)];
                        if (depthtg.w != this->w){return fbo_status_e::depth_mismatched_width;}
                        if (depthtg.h != this->h){return fbo_status_e::depth_mismatched_height;}
                    }
                    return fbo_status_e::ok;
                }

                bool attach(const fbo_attachment_e& atg, const unsigned int& texid, const textarget_e ttg=textarget_e::target2d) {
                    if (int(atg) < 0 || int(atg) >= this->attachments.size()){return false;}
                    auto& a = this->attachments[int(atg)];
                    a.target = ttg;
                    a.texture = texid;
                    return true;
                }

            };

            struct vertex_t {
                vec4 co = {0,0,0,1}, color = {1,1,1,1};
                vec3 normal = {0,0,1};
                vec2 uv[8] = {};
            };

            struct vao_t {
                TestContext* ctx = nullptr;
                unsigned int idx = 0;
                MemoryBuffer<vertex_t> vertices;
                MemoryBuffer<size_t> indices;
            };

            struct _drawtri_int_args {
                const vertex_t *pv = nullptr, *tv = nullptr, *ov = nullptr, *wv = nullptr, *opv = nullptr;
                const vec3* abcw = nullptr, *true_abcw = nullptr;
                texture_t *colortex = nullptr, *depthtex = nullptr;
                textarget_e colortg, depthtg;
                mat4 pvm, modelview, invproj;
                bool frontface;
                vec3 clipped_wvalues;
            };
            struct pixelshader_t {
                unsigned int idx = 0;
                TestContext* ctx = nullptr;
                void* userdata = nullptr;
                struct {
                    vec3 affine_bcw, pc_bcw, clipnormal, true_abcw, true_pbcw;
                    const vertex_t *pv = nullptr, *tv = nullptr, *ov = nullptr, *wv = nullptr, *opv = nullptr;
                    float z, original_z;
                    vec4 curfragcolor;
                    bool frontface;
                    mat4 pvm, modelview, invproj, mv_inv, pvm_inv, cam2world;
                    vec3 world_vertex_coords[3], world_vertex_normals[3];
                } f_in;
                struct {
                    vec4 fragcolor;
                    float fragdepth;
                } f_out;

                bool (*fmain)(pixelshader_t&) = nullptr;
            };
        private:
            unsigned int _curobjid = 1;
            unsigned int _width = 640, _height = 480;
            //std::valarray<rgb24_t> _rbuffer;
            //std::valarray<float> _zbuffer;
            std::unordered_map<unsigned int, texture_t*> _textures = {};
            std::unordered_map<unsigned int, pixelshader_t*> _pixelshaders = {};
            std::unordered_map<unsigned int, fbo_t*> _fbos = {};
            std::unordered_map<unsigned int, vao_t*> _vaos = {};
            unsigned int _cur_drawfboidx = 0;
            fbo_t* _cur_drawfbo = nullptr, *_default_fbo = nullptr;
            texture_t *_dcoltex = nullptr, *_ddepthtex = nullptr;
            std::vector<vec4> _pixel_row = {};
            std::vector<vec4> _depth_row = {};

            template <class VST, class IIT>
            class _indvertexiter_t {
                private:
                    VST* _vsp = nullptr;
                    IIT itr;
                public:
                    bool operator==(const _indvertexiter_t<VST,IIT>& other) const {return (this->_vsp == other._vsp && this->itr == other.itr);}
                    bool operator!=(const _indvertexiter_t<VST,IIT>& other) const {return (this->_vsp != other._vsp || this->itr != other.itr);}
                    bool operator<(const _indvertexiter_t<VST,IIT>& other) const {return (this->_vsp == other._vsp && this->itr < other.itr);}
                    _indvertexiter_t<VST,IIT> operator++(int){
                        auto cpy = *this;
                        this->itr++;
                        return cpy;
                    }
                    vertex_t& operator*() {return this->_vsp[0][*this->itr];}
                    const vertex_t& operator*() const {return this->_vsp[0][*this->itr];}
                    _indvertexiter_t() {}
                    _indvertexiter_t(VST& vertseq, IIT it) : _vsp(&vertseq), itr(it) {}
                    _indvertexiter_t(const _indvertexiter_t<VST,IIT>& o) : _vsp(o._vsp), itr(o.itr) {}
                    ~_indvertexiter_t() {}
            };


        public:
            const unsigned int& width = _width;
            const unsigned int& height = _height;

            struct {
                mat4 model, view, projection;
            } matrices = {mat4(1), mat4(1), mat4(1)};

            //inline rgb24_t* rbuffer() {return &(this->_rbuffer[0]);}
            //inline float* zbuffer() {return &(this->_zbuffer[0]);}

            inline void* rbuffer() {
                texture_t* tex = this->getTexture(this->_cur_drawfbo->attachments[0].texture);
                if (tex == nullptr){return nullptr;}
                return tex->targets[int(this->_cur_drawfbo->attachments[0].target)]._texels_u8.pfront();
            }
            inline void* zbuffer() {
                texture_t* tex = this->getTexture(this->_cur_drawfbo->attachments[1].texture);
                if (tex == nullptr){return nullptr;}
                return tex->targets[int(this->_cur_drawfbo->attachments[1].target)]._texels_u8.pfront();
            }

            bool isTexture(const unsigned int& idx) {return (this->_textures.count(idx) > 0);}

            unsigned int newTexture() {
                auto* nti = new texture_t();
                for (int i = 0; i < nti->targets.size(); i++){
                    nti->texImage2D(textarget_e(i), 1, 1, texelfmt_e::rgb8, nullptr);
                }
                this->_textures[nti->idx = this->_curobjid] = nti;
                return this->_curobjid++;
            }
            bool delTexture(const unsigned int& idx) {
                if (!this->isTexture(idx)){return false;}
                auto*& tex = this->_textures[idx];
                delete tex; tex = nullptr;
                this->_textures.erase(idx);
                return true;
            }

            texture_t* getTexture(const unsigned int& idx) {
                try {
                    return this->_textures.at(idx);
                } catch (const std::out_of_range& exc) {
                    return nullptr;
                }
            }

            bool isPixelShader(const unsigned int& idx) {return (this->_pixelshaders.count(idx) > 0);}
            unsigned int newPixelShader() {
                pixelshader_t* nps = new pixelshader_t();
                nps->ctx = this;
                this->_pixelshaders[nps->idx = this->_curobjid] = nps;
                return this->_curobjid++;
            }
            bool delPixelShader(const unsigned int& idx) {
                if (!this->isPixelShader(idx)){return false;}
                pixelshader_t*& o = this->_pixelshaders[idx];
                delete o; o = nullptr;
                this->_pixelshaders.erase(idx);
                return true;
            }

            pixelshader_t* getPixelShader(const unsigned int& idx) {
                try {
                    return this->_pixelshaders.at(idx);
                } catch (const std::out_of_range& exc){
                    return nullptr;
                }
            }


            bool isVAO(const unsigned int& idx) {
                return (this->_vaos.count(idx) > 0);
            }
            unsigned int newVAO() {
                auto* nvao = new vao_t();
                nvao->ctx = this;
                this->_vaos[nvao->idx = this->_curobjid] = nvao;
                return this->_curobjid++;
            }
            bool delVAO(const unsigned int& idx) {
                if (!this->isVAO(idx)){return false;}
                auto*& vao = this->_vaos[idx];
                delete vao; vao = nullptr;
                this->_vaos.erase(idx);
                std::cout << "[sw3d3]: deleted vao " << idx << ".\n";
                return true;
            }
            vao_t* getVAO(const unsigned int& idx) {
                try {
                    return this->_vaos.at(idx);
                } catch (const std::out_of_range& exc){
                    return nullptr;
                }
            }

            bool isFBO(const unsigned int& idx) {return (this->_fbos.count(idx) > 0);}
            unsigned int newFBO() {
                fbo_t* nfbo = new fbo_t();
                nfbo->ctx = this;
                this->_fbos[nfbo->idx = this->_curobjid] = nfbo;
                return this->_curobjid++;
            }
            bool delFBO(const unsigned int& idx) {
                if (!this->isFBO(idx)){return false;}
                fbo_t*& o = this->_fbos[idx];
                delete o; o = nullptr;
                this->_fbos.erase(idx);
                if (idx == this->_cur_drawfboidx){
                    this->setCurrentDrawFBO(0);
                }
                return true;
            }

            fbo_t* getFBO(const unsigned int& idx) {
                try {
                    return this->_fbos.at(idx);
                } catch (const std::out_of_range& exc){
                    return nullptr;
                }
            }

            unsigned int newFBO_CD(const unsigned int w, const unsigned int h,
                const texelfmt_e color_fmt=texelfmt_e::rgb8,
                const texelfmt_e depth_fmt=texelfmt_e::depth32f,
                const textarget_e color_target=textarget_e::target2d,
                const textarget_e depth_target=textarget_e::target2d
            ){
                auto* fbo = this->getFBO(this->newFBO());
                fbo->w = w; fbo->h = h;
                auto* coltex = this->getTexture(this->newTexture()); coltex->texImage2D(color_target, w, h, color_fmt, nullptr);
                auto* depthtex = this->getTexture(this->newTexture()); depthtex->texImage2D(depth_target, w, h, depth_fmt, nullptr);
                fbo->attach(fbo_attachment_e::color, coltex->idx, color_target);
                fbo->attach(fbo_attachment_e::depth, depthtex->idx, depth_target);
                return fbo->idx;
            }

            bool resizeFBO(const unsigned int fboid, const unsigned int nw, const unsigned int nh) {
                if (!this->isFBO(fboid)){return false;}
                auto* fbo = this->getFBO(fboid);
                fbo->w = nw;
                fbo->h = nh;
                for (auto& fboa : fbo->attachments) {
                    auto* atex = this->getTexture(fboa.texture);
                    if (atex == nullptr){continue;}
                    auto& atg = atex->targets[int(fboa.target)];
                    atex->texImage2D(fboa.target, nw, nh, atg.fmt, nullptr);
                }
                return true;
            }

            unsigned int getCurrentDrawFBO() {
                return this->_cur_drawfboidx;
            }

            bool setCurrentDrawFBO(const unsigned int& newidx) {
                if (newidx == 0) {
                    this->_cur_drawfboidx = (this->_default_fbo != nullptr ? this->_default_fbo->idx : 0);
                    this->_cur_drawfbo = this->_default_fbo;
                    this->_width = this->_cur_drawfbo->w;
                    this->_height = this->_cur_drawfbo->h;
                    this->_pixel_row.resize(this->_width);
                    this->_depth_row.resize(this->_width);
                    return true;
                }
                else if (!this->isFBO(newidx)){return false;}
                else {
                    this->_cur_drawfboidx = newidx;
                    this->_cur_drawfbo = this->_fbos[newidx];
                    this->_width = this->_cur_drawfbo->w;
                    this->_height = this->_cur_drawfbo->h;
                    this->_pixel_row.resize(this->_width);
                    this->_depth_row.resize(this->_width);
                    return true;
                }
            }


            vec4 clearcolor = vec4(0);
            float cleardepth = 1.0f;

            struct {
                vec4 diffuse = {1,1,1,1};
                unsigned int pixel_shader = 0;
                depthfunc_e depthfunc = depthfunc_e::less;
                bool color_mask = true, depth_mask = true;
                cullface_e cullface = cullface_e::disabled;
            } drawparams;

            struct {
                textarget_e target = textarget_e::target2d;
                texcoords_e coords = texcoords_e::uv0;
                unsigned int idx = 0;
                texture_t* _internalptr = nullptr;
            } active_textures[8];

            /*
            void setDimensions(const unsigned int nw, const unsigned int nh) {
                this->_width = nw;
                this->_height = nh;
                this->_rbuffer.resize(nw*nh);
                this->_zbuffer.resize(nw*nh);
                this->clearBuffers(true,true);
            }
            */
            void setDimensions(const unsigned int nw, const unsigned int nh) {
                this->_default_fbo->w = nw;
                this->_default_fbo->h = nh;
                this->_dcoltex->texImage2D(textarget_e::target2d, nw, nh, this->_dcoltex->targets[0].fmt, nullptr);
                this->_ddepthtex->texImage2D(textarget_e::target2d, nw, nh, this->_ddepthtex->targets[0].fmt, nullptr);
                this->setCurrentDrawFBO(0);
                this->clearBuffers(true,true);
            }



            void clearBuffers(const bool color=true, const bool depth=true) {
                //rgb24_t cc8 = rgb24_t(glm::clamp(vec3(this->clearcolor) * 255.0f, vec3(0), vec3(255)));
                size_t npx = this->_width * this->_height;
                if (color && this->isTexture(this->_cur_drawfbo->attachments[0].texture)){
                    this->getTexture(this->_cur_drawfbo->attachments[0].texture)->clear_texels(this->_cur_drawfbo->attachments[0].target, this->clearcolor);
                }
                if (depth && this->drawparams.depth_mask && this->isTexture(this->_cur_drawfbo->attachments[1].texture)){
                    this->getTexture(this->_cur_drawfbo->attachments[1].texture)->clear_texels(this->_cur_drawfbo->attachments[1].target, vec4(this->cleardepth));
                }
            }
            size_t _num_active_texslots = 0, _active_texslots[8] = {};
            void _get_active_texslots() {
                this->_num_active_texslots = 0;
                for (size_t i = 0; i < 8; i++){
                    auto& ts = this->active_textures[i];
                    ts._internalptr = nullptr;
                    if (this->isTexture(ts.idx)){
                        ts._internalptr = this->_textures[ts.idx];
                        this->_active_texslots[this->_num_active_texslots++] = i;
                    }
                }
            }

            template <typename T>
            static T _bclerp(const vec3& bcw, const T& a, const T& b, const T& c) {
                return ((a*bcw.x)+(b*bcw.y)+(c*bcw.z));
            }
        private:
            // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/perspective-correct-interpolation-vertex-attributes.html
            inline float _tri_edgefunc(const vec2 &a, const vec2 &b, const vec2 &c)
            { return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]); }


            inline float _tri_area(const vec2& a, const vec2& b, const vec2& c) {
                // https://en.wikipedia.org/wiki/Heron%27s_formula
                const float
                   sa = glm::distance(a, b),
                   sb = glm::distance(b, c),
                   sc = glm::distance(c, a),
                   s = 0.5f * (sa+sb+sc)
                ;
                return sqrt(s * (s-sa) * (s-sb) * (s-sc));
            }

            void _drawTriangle_internal(_drawtri_int_args& args) {
                const auto& tri_edges = _EDGEVERTINDICES;
                auto& pv0 = args.pv[0].co;
                auto& pv1 = args.pv[1].co;
                auto& pv2 = args.pv[2].co;

                auto z0 = args.opv[0].co.z;
                auto z1 = args.opv[1].co.z;
                auto z2 = args.opv[2].co.z;

                auto w0 = args.opv[0].co.w;
                auto w1 = args.opv[1].co.w;
                auto w2 = args.opv[2].co.w;

                const vec3 invwvalues = vec3(1) / vec3{w0, w1, w2};
                const vec3 true_invwvalues = vec3(1) / args.clipped_wvalues;
                //float tri_area = this->_tri_edgefunc(vec2(args.pv[0].co), vec2(args.pv[1].co), vec2(args.pv[2].co));
                ivec2 scr_min = {0,0}, scr_max = {this->_width-1, this->_height-1};
                ivec2 ipvco[3];
                vec2 fpvco[3];
                vec2 scr_maxf(scr_max);
                for (int i = 0; i < 3; i++){
                    fpvco[i] = scr_maxf * (vec2(args.pv[i].co * 0.5f) + 0.5f);
                    ipvco[i] = ivec2(fpvco[i]);
                }
                //float tri_area = this->_tri_area(vec2(fpvco[0]), vec2(fpvco[1]), vec2(fpvco[2]));
                ivec2 tri_min = glm::clamp(iv2min3(ipvco[0], ipvco[1], ipvco[2]), scr_min, scr_max);
                ivec2 tri_max = glm::clamp(iv2max3(ipvco[0], ipvco[1], ipvco[2]), scr_min, scr_max);
                ivec2 xy = tri_min;
                const vec4 one(1);
                pixelshader_t* ps = this->getPixelShader(this->drawparams.pixel_shader);
                bool isps = (ps != nullptr);
                bool ps_hasfmain = (isps && ps->fmain != nullptr);
                const vec3 clipnormal = glm::triangleNormal(vec3(args.tv[0].co),vec3(args.tv[1].co),vec3(args.tv[2].co));
                //const bool frontface = (glm::dot(clipnormal, vec3{0,0,1}) > 0.0f);
                const bool frontface = args.frontface;
                //if (!frontface){return;}
                if (isps) {
                    ps->f_in.cam2world = glm::inverse(glm::inverse(this->matrices.projection) * this->matrices.view);
                }
                texture_t::_target* colortarget = (args.colortex != nullptr ? &(args.colortex->targets[0]) : nullptr);
                texture_t::_target* depthtarget = (args.depthtex != nullptr ? &(args.depthtex->targets[0]) : nullptr);
                for (; xy.y <= tri_max.y; xy.y++){

                    drawspan_t span = {};
                    bool got_span = false;
                    for (int i = 0; i < 3; i++){
                        const auto& tv0 = fpvco[tri_edges[i][0]];
                        const auto& tv1 = fpvco[tri_edges[i][1]];


                        if (span.try_isect(i, xy.y, tv0[0],tv0[1], tv1[0],tv1[1])){
                            got_span = true;
                            span.calc_span(INT_MIN, INT_MIN, INT_MAX, INT_MAX);
                            break;
                        }
                    }
                    if (!got_span){continue;}
                    auto& edge0 = tri_edges[span.edge0];
                    auto& edge1 = tri_edges[span.edge1];
                    auto abcw0 = glm::mix(args.abcw[edge0[0]], args.abcw[edge0[1]], span.edge0fac);
                    auto abcw1 = glm::mix(args.abcw[edge1[0]], args.abcw[edge1[1]], span.edge1fac);

                    auto tabcw0 = glm::mix(args.true_abcw[edge0[0]], args.true_abcw[edge0[1]], span.edge0fac);
                    auto tabcw1 = glm::mix(args.true_abcw[edge1[0]], args.true_abcw[edge1[1]], span.edge1fac);

                    int xmin = glm::clamp(span.x0i, tri_min.x, tri_max.x), xmax = glm::clamp(span.x1i, tri_min.x, tri_max.x);
                    if (xmax <= xmin){continue;}
                    //std::cout << xmin << "," << xmax << "\n";
                    /*
                    pixelrowcoord_t prc_xstart(xmin, scr_max.y - xy.y);
                    pixelrowcoord_t prc_xend(xmax+1, scr_max.y - xy.y);
                    if (args.depthtex != nullptr){

                        args.depthtex->get_texels_for_xys(args.depthtg, this->_depth_row.begin(), prc_xstart, prc_xend);
                    }
                    if (args.colortex != nullptr){
                        args.colortex->get_texels_for_xys(args.colortg, this->_pixel_row.begin(), prc_xstart, prc_xend);
                    }
                    bool row_dirty = false;
                    */



                    for (xy.x = xmin; xy.x <= xmax; xy.x++){
                        int xdelta = xy.x - xmin;
                        //vec4& pixel_row_elem = this->_pixel_row[xdelta];
                        //vec4& depth_row_elem = this->_depth_row[xdelta];

                        float lerpfac = float(xy.x - span.x0i) / float(span.x1i - span.x0i);

                        const ivec2 xy2 = {
                            xy.x,
                            scr_max.y - xy.y
                        };
                        auto abcw = glm::mix(abcw0, abcw1, vec3(lerpfac));
                        auto true_abcw = glm::mix(tabcw0, tabcw1, vec3(lerpfac));
                        float z = _bclerp(abcw, z0, z1, z2);
                        size_t pxidx = (this->_width * (scr_max.y - xy.y)) + xy.x;

                        vec4 pixel_row_elem = vec4(0), depth_row_elem = vec4(1);
                        if (colortarget != nullptr){colortarget->_gettexel4idx(pixel_row_elem, pxidx);}
                        if (depthtarget != nullptr){depthtarget->_gettexel4idx(depth_row_elem, pxidx);}

                        //if (z >= this->_zbuffer[pxidx] || z < -1 || z > 1){continue;}
                        if (z < -1 || z > 1 || !_z_compare(this->drawparams.depthfunc, z, depth_row_elem.r)){continue;}

                        vec4 final_fragcolor = {0,0,0,1};


                        if (isps){
                            ps->f_in.affine_bcw = abcw;
                            ps->f_in.pc_bcw = abcw * invwvalues; ps->f_in.pc_bcw /= glm::compAdd(ps->f_in.pc_bcw);
                            ps->f_in.true_abcw = true_abcw;
                            ps->f_in.true_pbcw = true_abcw * true_invwvalues; ps->f_in.true_pbcw /= glm::compAdd(ps->f_in.true_pbcw);
                            ps->f_in.pv = args.pv;
                            ps->f_in.tv = args.tv;
                            ps->f_in.ov = args.ov;
                            ps->f_in.opv = args.opv;
                            ps->f_in.wv = args.wv;
                            ps->f_in.original_z = depth_row_elem.r;
                            ps->f_out.fragdepth = ps->f_in.z = z;
                            ps->f_in.curfragcolor = vec4(0);
                            ps->f_in.clipnormal = clipnormal;
                            ps->f_in.frontface = frontface;
                            ps->f_in.curfragcolor = pixel_row_elem;
                            ps->f_in.pvm = args.pvm;


                            if (ps_hasfmain){
                                if (!ps->fmain(*ps)){continue;} //discard the fragment if returned false;
                            }
                            final_fragcolor = ps->f_out.fragcolor;
                            depth_row_elem.r = ps->f_out.fragdepth;
                        }
                        else {
                            depth_row_elem.r = z;
                        }

                        //this->_rbuffer[pxidx] = rgb24_t(glm::clamp(vec3(final_fragcolor) * 255.0f, vec3(0), vec3(255)));
                        //this->_zbuffer[pxidx] = z;
                        pixel_row_elem = final_fragcolor;
                        //depth_row_elem.r = z;
                        if (colortarget != nullptr && this->drawparams.color_mask){colortarget->_settexel4idx(pxidx, pixel_row_elem);}
                        if (depthtarget != nullptr && this->drawparams.depth_mask){depthtarget->_settexel4idx(pxidx, depth_row_elem);}
                        //row_dirty = true;
                    }
                    /*
                    if (row_dirty) {
                        if (args.depthtex != nullptr && this->drawparams.depth_mask){
                            args.depthtex->set_texels_for_xys(args.depthtg, prc_xstart, prc_xend, this->_depth_row.begin());
                        }
                        if (args.colortex != nullptr && this->drawparams.color_mask){
                            args.colortex->set_texels_for_xys(args.colortg, prc_xstart, prc_xend, this->_pixel_row.begin());
                        }
                    }
                    */
                }
            }
        public:
            template <class ITR>
            size_t drawTriangles(ITR sbeg, ITR send) {
                this->_get_active_texslots();
                size_t tricount = 0;
                mat4 pvm = this->matrices.projection * glm::inverse(this->matrices.view) * this->matrices.model;
                mat4 mv = glm::inverse(this->matrices.view) * this->matrices.model;
                mat4 nmat = glm::transpose(glm::inverse(mv));
                const vec3 camdir_world = glm::normalize(vec3(this->matrices.view * vec4{0,0,-1,0}));
                const vec3 campos_world(this->matrices.view[3]);
                _drawtri_int_args dtia;
                dtia.pvm = pvm;
                dtia.modelview = mv;
                dtia.invproj = glm::inverse(this->matrices.projection);
                float znear, zfar;
                const vec4 znear_unproj = dtia.invproj * vec4{0,0,-1,1}; znear = znear_unproj.z / znear_unproj.w;
                const vec4 zfar_unproj = dtia.invproj * vec4{0,0,1,1}; zfar = -zfar_unproj.z / zfar_unproj.w;

                //if (zfar > znear){std::swap(znear, zfar);}
                pixelshader_t* ps = this->getPixelShader(this->drawparams.pixel_shader);
                bool isps = (ps != nullptr);

                if (this->isTexture(this->_cur_drawfbo->attachments[0].texture)){
                    dtia.colortex = this->getTexture(this->_cur_drawfbo->attachments[0].texture);
                    dtia.colortg = this->_cur_drawfbo->attachments[0].target;
                }
                if (this->isTexture(this->_cur_drawfbo->attachments[1].texture)){
                    dtia.depthtex = this->getTexture(this->_cur_drawfbo->attachments[1].texture);
                    dtia.depthtg = this->_cur_drawfbo->attachments[1].target;
                }
                for (ITR cv = sbeg; cv < send;){
                    vertex_t overts[3] = {}, tverts[3] = {}, vverts[3] = {}, pverts[3] = {}, wverts[3] = {}, spverts[3] = {}, svverts[3] = {};
                    size_t nve = 0;
                    for (; nve < 3 && cv < send; nve++, cv++){
                        overts[nve] = *cv;
                    }
                    if (nve != 3){break;}

                    size_t nva = 0;
                    vec4 prjcos[3] = {};
                    for (int i = 0; i < 3; i++){
                        auto& ov = overts[i];
                        auto& wv = wverts[i];
                        wv = ov;
                        wv.co = this->matrices.model * ov.co;
                        wv.normal = glm::normalize(vec3(this->matrices.model * vec4(ov.normal, 0)));

                        auto& vv = vverts[i];
                        vv = ov;
                        vv.co = mv * ov.co;

                        vv.normal = glm::normalize(vec3(mv * vec4(ov.normal, 0)));

                        if (vv.co.z <= 0.0f){nva++;}

                    }

                    if (nva == 0){continue;}

                    //const vec3 campos_world(this->matrices.view[3]);
                    const vec3 campos_view = {0,0,0};
                    const vec3 facenormal = glm::normalize(glm::triangleNormal(
                        vec3(vverts[0].co),
                        vec3(vverts[1].co),
                        vec3(vverts[2].co)
                    ));
                    const bool frontface = (glm::dot(glm::normalize(-vec3(vverts[0].co)), facenormal) >= 0.0f);
                    switch (this->drawparams.cullface) {
                        case cullface_e::back: if (!frontface){continue;} break;
                        case cullface_e::front: if (frontface){continue;} break;
                        default: ;
                    }

                    for (int i = 0; i < 3; i++){
                        auto& tv = tverts[i];
                        auto& vv = vverts[i];
                        auto& ov = overts[i];
                        tv = vv;
                        //vec4 prjco = this->matrices.projection * vec4(vec3(vv.co), 1);
                        vec4 prjco = pvm * vec4(vec3(ov.co), 1);
                        prjcos[i] = prjco;
                        tv.co = vec4(vec3(prjco) / prjco.w, prjco.w);
                        tv.normal = glm::normalize(vec3(this->matrices.projection * vec4(vv.normal, 0)));
                    }

                    size_t nback, nfront;
                    _tri_bisect::cliptriangle_t _backtris[2], _fronttris[2];

                    _tri_bisect::clip_tri_plane(nback, _backtris, nfront, _fronttris, vec3{0,0,znear}, (znear < zfar ? vec3{0,0,-1} : vec3{0,0,1}), vec3(vverts[0].co),vec3(vverts[1].co),vec3(vverts[2].co));

                    for (size_t fti = 0; fti < nfront; fti++){
                        _tri_bisect::cliptriangle_t& cliptri = _fronttris[fti];
                        _tri_bisect::clipvertex_t* clipverts = &cliptri.v0;
                        const vec3 abcw[3] = {{1,0,0}, {0,1,0}, {0,0,1}};
                        vec3 true_abcw[3] = {};
                        for (int i = 0; i < 3; i++){
                            auto& cv = clipverts[i];
                            auto& pv = pverts[i];
                            auto& tv = tverts[i];
                            pv = vverts[i];
                            dtia.clipped_wvalues[i] = _bclerp(cv.barycentric_weights, tverts[0].co.w, tverts[1].co.w, tverts[2].co.w);
                            true_abcw[i] = _bclerp(cv.barycentric_weights, abcw[0], abcw[1], abcw[2]);
                            //pverts[i] = tv;
                            vec4 prjco = this->matrices.projection * vec4(cv.co, 1);
                            pv.co = vec4(vec3(prjco) / prjco.w, prjco.w);
                            #define _lerpfield(_n) pv._n = _bclerp(cv.barycentric_weights, vverts[0]._n,vverts[1]._n,vverts[2]._n)
                            _lerpfield(color);
                            _lerpfield(normal); pv.normal = glm::normalize(vec3(this->matrices.projection * vec4(pv.normal, 0)));
                            //pv.normal = glm::normalize(_bclerp(cv.barycentric_weights, tverts[0].normal,tverts[1].normal,tverts[2].normal));
                            _lerpfield(uv[0]);
                            _lerpfield(uv[1]);
                            _lerpfield(uv[2]);
                            _lerpfield(uv[3]);
                            _lerpfield(uv[4]);
                            _lerpfield(uv[5]);
                            _lerpfield(uv[6]);
                            _lerpfield(uv[7]);
                            #undef _lerpfield
                            if (isps){
                                ps->f_in.world_vertex_coords[i] = _bclerp(cv.barycentric_weights, vec3(wverts[0].co), vec3(wverts[1].co), vec3(wverts[2].co));
                                ps->f_in.world_vertex_normals[i] = glm::normalize(_bclerp(cv.barycentric_weights, vec3(wverts[0].normal), vec3(wverts[1].normal), vec3(wverts[2].normal)));
                            }

                        }
                        std::copy_n(pverts, 3, spverts);

                        dtia.ov = overts;
                        dtia.tv = tverts;
                        dtia.pv = spverts;
                        dtia.opv = pverts;
                        dtia.wv = wverts;
                        dtia.abcw = abcw;
                        dtia.true_abcw = true_abcw;
                        dtia.frontface = frontface;
                        this->_drawTriangle_internal(dtia);
                        tricount++;
                    }

                }
                return tricount;
            }

            size_t drawTriangles(const unsigned int& vaoidx, const size_t maxnum=size_t(-1)){
                if (!this->isVAO(vaoidx)){return 0;}
                auto* vao = this->getVAO(vaoidx);
                if (vao->vertices.size() == 0){return 0;}
                auto* vstart = vao->vertices.pfront();
                return this->drawTriangles(vstart, vstart + std::min(maxnum, vao->vertices.size()));
            }

            size_t drawTrianglesIndexed(const unsigned int& vaoidx, const size_t maxnum=size_t(-1)){
                if (!this->isVAO(vaoidx)){return 0;}
                auto* vao = this->getVAO(vaoidx);
                if (vao->vertices.size() == 0 || vao->indices.size() == 0){return 0;}
                auto* ipstart = vao->indices.pfront();
                _indvertexiter_t<MemoryBuffer<vertex_t>, size_t*> ibegin(vao->vertices, ipstart);
                _indvertexiter_t<MemoryBuffer<vertex_t>, size_t*> iend(vao->vertices, ipstart + std::min(maxnum, vao->indices.size()));
                return this->drawTriangles(ibegin, iend);
            }

            TestContext() {
                this->_cur_drawfboidx = this->newFBO();
                this->_cur_drawfbo = this->_default_fbo = this->getFBO(this->_cur_drawfboidx);
                texture_t *fcoltex = this->getTexture(this->newTexture()), *fdepthtex = this->getTexture(this->newTexture());
                this->_dcoltex = fcoltex;
                this->_ddepthtex = fdepthtex;
                fcoltex->texImage2D(textarget_e::target2d, 1, 1, texelfmt_e::rgb8, nullptr);
                fdepthtex->texImage2D(textarget_e::target2d, 1, 1, texelfmt_e::depth32f, nullptr);
                this->_cur_drawfbo->w = this->_width = 1;
                this->_cur_drawfbo->h = this->_height = 1;
                this->_cur_drawfbo->attach(fbo_attachment_e::color, fcoltex->idx);
                this->_cur_drawfbo->attach(fbo_attachment_e::depth, fdepthtex->idx);

                this->_pixel_row.resize(1);
                this->_depth_row.resize(1);

            }
            ~TestContext() {
                while (this->_pixelshaders.size() > 0){
                    this->delPixelShader(this->_pixelshaders.begin()->first);
                }
                this->_pixelshaders.clear();
                while (this->_textures.size() > 0){
                    this->delTexture(this->_textures.begin()->first);
                }
                this->_textures.clear();
                while (this->_fbos.size() > 0){
                    this->delFBO(this->_fbos.begin()->first);
                }
                this->_fbos.clear();
                while (this->_vaos.size() > 0){
                    this->delVAO(this->_vaos.begin()->first);
                }
                this->_vaos.clear();
            }
    };

    typedef TestContext::texture_t texture_t;
    typedef TestContext::fbo_t fbo_t;
    typedef TestContext::vao_t vao_t;
    typedef TestContext::vertex_t vertex_t;
    typedef TestContext::pixelshader_t pixelshader_t;
    typedef TestContext::textarget_e textarget_e;
    typedef TestContext::texcoords_e texcoords_e;
    typedef TestContext::texwrap_e texwrap_e;
    typedef TestContext::texfilter_e texfilter_e;
    typedef TestContext::texelfmt_e texelfmt_e;
    typedef TestContext::fbo_status_e fbo_status_e;
    typedef TestContext::fbo_attachment_e fbo_attachment_e;
    typedef TestContext::cullface_e cullface_e;
    typedef TestContext::depthfunc_e depthfunc_e;
}


