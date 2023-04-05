#ifndef DJ_SW3D20_HPP_INCLUDED
#define DJ_SW3D20_HPP_INCLUDED

#ifdef DJ_SW3D20_USE_OPENMP
 #include <omp.h>
#endif

//#include <type_traits>

#include <iostream>
#include <istream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include <exception>
#include <sstream>

#include <cmath>
#include <cstring>
#include <cstdio>

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

#include "glm/ext/scalar_common.hpp"
#include "glm/ext/vector_common.hpp"
#include "glm/gtc/integer.hpp"
#include "glm/ext/matrix_projection.hpp"
//#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/intersect.hpp"
#include "glm/gtx/fast_trigonometry.hpp"
#include "glm/gtx/wrap.hpp"
//#include "glm/detail/func_vector_relational.hpp"

namespace dj_sw3d20
{
    #if (!defined(DJ_SW3D20_FPSCALAR_TYPE))
     using fpscalar = double;
    #else
     using fpscalar = DJ_SW3D20_FPSCALAR_TYPE;
    #endif
    #ifndef DJ_SW3D20_MAX_TEXTURE_MIPMAPS
     #define DJ_SW3D20_MAX_TEXTURE_MIPMAPS 16
    #endif
    #ifndef DJ_SW3D20_MATERIAL_MAX_TEXSLOTS
     #define DJ_SW3D20_MATERIAL_MAX_TEXSLOTS 16
    #endif
    using uint = unsigned int;
    using glm::ivec2;
    using glm::ivec3;
    using glm::ivec4;

    using glm::uvec2;
    using glm::uvec3;
    using glm::uvec4;

    using glm::imat2;
    using glm::imat3;
    using glm::imat4;

    template <glm::length_t L, glm::qualifier Q=glm::defaultp> using _fpvecNQ = glm::vec<L, fpscalar, Q>;
    template <glm::length_t L, glm::qualifier Q=glm::defaultp> using _u8vecNQ = glm::vec<L, uint8_t, Q>;
    template <glm::length_t L, glm::qualifier Q=glm::defaultp> using _uivecNQ = glm::vec<L, uint, Q>;
    template <glm::length_t L, glm::qualifier Q=glm::defaultp> using _ivecNQ = glm::vec<L, int, Q>;

    template <glm::length_t L, class T, glm::qualifier Q=glm::defaultp> using _tvecNQ = glm::vec<L, T, Q>;

    template <glm::length_t L, glm::qualifier Q=glm::defaultp> using _fpmatNNQ = glm::mat<L, L, fpscalar, Q>;
    template <glm::length_t L, class T, glm::qualifier Q=glm::defaultp> using _tmatNNQ = glm::mat<L, L, T, Q>;

    template <glm::length_t L, glm::qualifier Q=glm::defaultp> using _aabbNQi = glm::mat<2, L, int, Q>;
    template <glm::length_t L, glm::qualifier Q=glm::defaultp> using _aabbNQfp = glm::mat<2, L, fpscalar, Q>;

    template <glm::length_t L, class T, glm::qualifier Q=glm::defaultp> using _aabbNQT = glm::mat<2, L, T, Q>;

    namespace fpvectypes
    {
	    using fpvec2 = _fpvecNQ<2>;
	    using fpvec3 = _fpvecNQ<3>;
	    using fpvec4 = _fpvecNQ<4>;
    }
    namespace fpmattypes
    {
	    using fpmat2 = glm::mat<2, 2, fpscalar, glm::defaultp>;
	    using fpmat3 = glm::mat<3, 3, fpscalar, glm::defaultp>;
	    using fpmat4 = glm::mat<4, 4, fpscalar, glm::defaultp>;
    }
    namespace aabbtypes
    {
	    using aabb2Di = glm::imat2;
	    using aabb3Di = glm::imat2x3;
	    using aabb4Di = glm::imat2x4;

	    using aabb2Dfp = glm::mat<2, 2, fpscalar, glm::defaultp>;
	    using aabb3Dfp = glm::mat<2, 3, fpscalar, glm::defaultp>;
	    using aabb4Dfp = glm::mat<2, 4, fpscalar, glm::defaultp>;
    }
    namespace u8colortypes
    {
	    /*
	    using u8ra = glm::vec<2, uint8_t, glm::defaultp>;
	    using u8rgb = glm::vec<3, uint8_t, glm::defaultp>;
	    using u8rgba = glm::vec<4, uint8_t, glm::defaultp>;
	    */
	    using u8ra = _u8vecNQ<2>;
	    using u8rgb = _u8vecNQ<3>;
	    using u8rgba = _u8vecNQ<4>;
	}
    using namespace dj_sw3d20::fpvectypes;
    using namespace dj_sw3d20::fpmattypes;
    using namespace dj_sw3d20::aabbtypes;
    using namespace dj_sw3d20::u8colortypes;

    enum class enum_t : int {

    	NEAREST = 0, //nearest-neighbor filtering, no mipmap (aka: nearest-neighbor filtering)
    	LINEAR = 1, //bilinear filtering, no mipmap (aka: bilinear filtering)
    	NEAREST_MIPMAP_NEAREST = 2, //nearest-neighbor filtering, sharp switch between mipmap levels.
    	NEAREST_MIPMAP_LINEAR = 3, //nearest-neighbor filtering, smooth switch between mipmap levels.
    	LINEAR_MIPMAP_NEAREST = 4, //bilinear filtering, sharp switch between mipmap levels.
    	LINEAR_MIPMAP_LINEAR = 5, //bilinear filtering, smooth switch between mipmap levels. (aka: trilinear filtering)

    	GRAYSCALE = 1,
    	GRAYALPHA = 2,
    	RGB = 3,
    	RGBA = 4,
    	DEPTH_COMPONENT = 5,

    	COLOR_BUFFER = 1,
    	DEPTH_BUFFER = 2,

    	REPEAT = 0,
    	CLAMP_TO_EDGE = 1,

    	DRAW_POINTS = 0,
    	DRAW_LINES = 1,
    	DRAW_TRIANGLES = 2,

    	TRISTYLE_FILL = 0,
    	TRISTYLE_WIREFRAME = 1,

    	CAP_FACE_CULLING = 1,
    	CAP_DEPTH_TEST = 2,
    	CAP_DRAW_COLOR_BUFFER_R = 3,
    	CAP_DRAW_COLOR_BUFFER_G = 4,
    	CAP_DRAW_COLOR_BUFFER_B = 5,
    	CAP_DRAW_COLOR_BUFFER_A = 6,
    	CAP_DRAW_DEPTH_BUFFER   = 7,
		CAP_BLENDING = 8,

    	BACK_FACE = 0,
    	FRONT_FACE = 1,

    	BLEND_MIX = 1,
    	BLEND_ADD = 2,
    	BLEND_SUB = 3,
    	BLEND_MUL = 4,
    	BLEND_DIV = 5,

		SRC_ALPHA = 0,
		ONE_MINUS_SRC_ALPHA = 1,

    	CMP_LESS = 0,
    	CMP_LEQUAL = 1,
    	CMP_GREATER = 2,
    	CMP_GEQUAL = 3,
    	CMP_EQUAL = 4,
    	CMP_NOTEQUAL = 5,

    	TEXTYPE_IMAGE = 0,
    	TEXTYPE_ENVMAP = 1,

    	ENVMAP_EQUIRECT = 0,
    	ENVMAP_CUBEMAP = 1,


    	TEXMAP_UVMAP = 0,
    	TEXMAP_VIEWVECTOR = 1,
    	TEXMAP_REFLECTION = 2,
    	TEXMAP_SCREEN_01 = 3,

    	ENVTG_EQUIRECT = 0,
    	ENVTG_CUBEMAP_NEGX = 1,
    	ENVTG_CUBEMAP_POSX = 2,
    	ENVTG_CUBEMAP_NEGY = 3,
    	ENVTG_CUBEMAP_POSY = 4,
    	ENVTG_CUBEMAP_NEGZ = 5,
    	ENVTG_CUBEMAP_POSZ = 6,

    	PROJECTION_MATRIX = 0,
    	VIEW_MATRIX = 1,
    	MODEL_MATRIX = 2,

    	BUMPMAPSPACE_OBJECT = 0,
    	BUMPMAPSPACE_TANGENT = 1,

    	_GET_CURRENT_VALUE = -1,
    	NONE = 0
    };

    const size_t _drawtype_primitive_sizes[] = {
		1, //DRAW_POINTS
		2, //DRAW_LINES
		3 //DRAW_TRIANGLES
    };
    struct _ContextManageableObject;

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
                this->_buffer = b;
                this->_owns_buffer = false;
            }

            inline ET& operator[](const size_t idx){return this->_buffer[idx];}
            inline ET& front() {return this->_buffer[0];}
            inline ET& back() {return this->_buffer[this->_bufnelems - 1];}

            inline ET* pfront() {return &this->front();}
            inline ET* pback() {return &this->back();}

            inline operator ET*() {return this->_buffer;}

            MemoryBuffer() {}
            MemoryBuffer(const size_t n, const ET* b) {this->setsharedbuf(n,b);}
            ~MemoryBuffer() {this->reset();}
    };

    using _ctxobjmap_t = std::unordered_map<size_t, _ContextManageableObject*>;

    struct _ContextManageableObject
    {
		private:
		    size_t _ctxobjid = 0;
		    _ctxobjmap_t* _ctxobjmap = nullptr;
		public:
		    inline void _cmo_setup(_ctxobjmap_t& coll, const size_t newid)
		    {
				coll[newid] = this;
				this->_ctxobjid = newid;
				this->_ctxobjmap = &coll;
			}
			inline void _cmo_delete()
			{
				this->on_destroy();
				if (this->_ctxobjmap != nullptr && this->_ctxobjid != 0)
				{
					this->_ctxobjmap->erase(this->_ctxobjid);
				}
				this->_ctxobjid = 0;
				this->_ctxobjmap = nullptr;
			}

		    virtual void on_destroy() {}
		    virtual void on_create() {}
		    _ContextManageableObject() {this->on_create();}
		    _ContextManageableObject(_ctxobjmap_t& coll, const size_t newid) {this->_cmo_setup(coll, newid); this->on_create();}
		    ~_ContextManageableObject() {this->_cmo_delete();}
	};

    namespace _math
    {
        const fpvec3 _ZERO_FPV3(0);
        const fpvec3 _ONE_FPV3(1);
        const fpvec2 _HALF_FPV2(fpscalar(1)/fpscalar(2));
        const u8rgb _U8RGB_WHITE(255U);
        const u8rgba _U8RGBA_WHITE(255U);
        const fpscalar _ZERO(0);
        const fpscalar _HALF(0.5);
        const fpscalar _ONE(1);
        const fpscalar _FP255(255);
        const fpscalar _FPDIV255(1.0/255.0);
        const u8rgb _LUM_WEIGHTS_U8 = {76U, 150U, 29U};
        const uvec3 _LUM_WEIGHTS_UI = {76U, 150U, 29U};
        const fpvec3 _LUM_WEIGHTS_FP = {0.299, 0.587, 0.114};
        const fpscalar PI_FP = glm::pi<fpscalar>();
        const fpscalar INV_PI_FP = _ONE/PI_FP;
        const fpscalar TWOPI_FP = PI_FP+PI_FP;
        const fpscalar INV_TWOPI_FP = _ONE/TWOPI_FP;
        using glm::unpackUnorm;
        using glm::unpackUnorm1x8;
        using glm::packUnorm1x8;
        using glm::packUnorm;

        inline int longest_vec3_axis(const fpvec3& v){
            int la = 0;
            if (abs(v.y) > abs(v[la])){la = 1;}
            if (abs(v.z) > abs(v[la])){la = 2;}
            return la;
        }

        enum_t vec2cubeface(const fpvec3& v){
            int vla = longest_vec3_axis(v), _add = 0;
            if (vla == 0){_add = (v.x >= 0.0f) ? 1 : 0;}
            else if (vla == 1){_add = (v.y >= 0.0f) ? 1 : 0;}
            else if (vla == 2){_add = (v.z >= 0.0f) ? 1 : 0;}
            return enum_t(int(enum_t::ENVTG_CUBEMAP_NEGX)+(2*vla)+_add);
        }

        void vec2cubemap_old(const fpvec3& v, enum_t& face, fpvec2& uvloc){
            face = vec2cubeface(v);
            const fpvec3 nv = glm::normalize(v)*1.41421356237;
            if (face == enum_t::ENVTG_CUBEMAP_NEGX){uvloc = (fpvec2{nv.y, nv.z}*fpvec2(0.5))+fpvec2(0.5);}
            else if (face == enum_t::ENVTG_CUBEMAP_POSX){uvloc = fpvec2(1.0-((nv.y*0.5)+0.5), (nv.z*0.5)+0.5);}
            else if (face == enum_t::ENVTG_CUBEMAP_NEGY){uvloc = fpvec2(1.0-((nv.x*0.5)+0.5), (nv.z*0.5)+0.5);}
            else if (face == enum_t::ENVTG_CUBEMAP_POSY){uvloc = (fpvec2{nv.x, nv.z}*fpvec2(0.5))+fpvec2(0.5);}
            else if (face == enum_t::ENVTG_CUBEMAP_NEGX){uvloc = (fpvec2{nv.x, nv.y}*fpvec2(0.5))+fpvec2(0.5);}
            else if (face == enum_t::ENVTG_CUBEMAP_POSZ){uvloc = fpvec2((nv.x*0.5)+0.5, 1.0-((nv.y*0.5)+0.5));}
        }

        void vec2cubemap(const fpvec3& v, enum_t& face, fpvec2& uvloc)
        {
            fpvec2 uvc = {};
            fpvec3 nv = glm::normalize(v)*fpvec3(1.41421356237);
            face = vec2cubeface(nv);
            float ax = abs(nv.x), ay = abs(nv.y), az = abs(nv.z), maxaxis = ax;
            switch (face)
            {
                case enum_t::ENVTG_CUBEMAP_NEGX: maxaxis = ax; uvc = fpvec2(nv.y, nv.z); break;
                case enum_t::ENVTG_CUBEMAP_POSX: maxaxis = ax; uvc = fpvec2(-nv.y, nv.z); break;

                case enum_t::ENVTG_CUBEMAP_NEGY: maxaxis = ay; uvc = fpvec2(-nv.x, nv.z); break;
                case enum_t::ENVTG_CUBEMAP_POSY: maxaxis = ay; uvc = fpvec2(nv.x, nv.z); break;

                case enum_t::ENVTG_CUBEMAP_NEGZ: maxaxis = az; uvc = fpvec2(nv.x, nv.y); break;
                case enum_t::ENVTG_CUBEMAP_POSZ: maxaxis = az; uvc = fpvec2(nv.x, -nv.y); break;
            }
            uvloc = glm::clamp(fpvec2(
                0.5 * (uvc.x / maxaxis + 1.0),
                0.5 * (uvc.y / maxaxis + 1.0)
            ), fpvec2(0.0), fpvec2(1.0));
        }

        inline fpvec3 tri_normal(const fpvec3& a, const fpvec3& b, const fpvec3& c) {
			return glm::normalize(glm::cross(b-a, c-a));
		}

        inline fpvec2 equirect2uv(const fpvec3& vec)
        {
        	const fpscalar vl = _ONE/glm::length(vec);
        	return {
        			(_HALF)+(atan2((vec.x*vl), (vec.y*vl))*INV_TWOPI_FP),
    				(_HALF)-(asin(-vec.z*vl)*INV_PI_FP)
    	    };
        }
        template <glm::length_t L, class T, glm::qualifier Q=glm::defaultp>
        void tri_aabb(_aabbNQT<L,T,Q>& outbox, const glm::vec<L,T,Q>& v0, const glm::vec<L,T,Q>& v1, const glm::vec<L,T,Q>& v2)
        {
        	outbox[0] = glm::min(v0, glm::min(v1, v2));
        	outbox[1] = glm::max(v0, glm::max(v1, v2));
        }
        template <glm::length_t L, class T, glm::qualifier Q=glm::defaultp>
        _aabbNQT<L,T,Q> tri_aabb(const glm::vec<L,T,Q>& v0, const glm::vec<L,T,Q>& v1, const glm::vec<L,T,Q>& v2)
        {
        	_aabbNQT<L,T,Q> res; tri_aabb(res, v0, v1, v2);
        	return res;
        }
        template <glm::length_t L, class T, glm::qualifier Q=glm::defaultp>
        void clamp_rect(_aabbNQT<L,T,Q>& rect, const _aabbNQT<L,T,Q>& bounds)
        {
        	rect[0] = glm::clamp(rect[0], bounds[0], bounds[1]);
        	rect[1] = glm::clamp(rect[1], bounds[0], bounds[1]);
        }
        template <glm::length_t L, class T, glm::qualifier Q=glm::defaultp>
        _aabbNQT<L,T,Q> clamp_rect(const _aabbNQT<L,T,Q>& rect, const _aabbNQT<L,T,Q>& bounds)
        {
        	_aabbNQT<L,T,Q> res = rect; clamp_rect(res, bounds);
        	return res;
        }

        template <class T, glm::qualifier Q=glm::defaultp>
        inline bool rect2ds_overlap(const _aabbNQT<2,T,Q>& r1, const _aabbNQT<2,T,Q>& r2)
        {
        	return (
        	    (r1[0].x <= r2[1].x && r1[1].x >= r2[0].x) &&
        	    (r1[0].y <= r2[1].y && r1[1].y >= r2[0].y)
        	);
        }

        template <class VT>
        bool valcmp(const enum_t op, const VT a, const VT b)
        {
			switch (op)
			{
				case enum_t::CMP_LESS: return (a < b);
				case enum_t::CMP_LEQUAL: return (a <= b);
				case enum_t::CMP_GREATER: return (a > b);
				case enum_t::CMP_GEQUAL: return (a >= b);
				case enum_t::CMP_EQUAL: return (a == b);
				case enum_t::CMP_NOTEQUAL: return (a != b);
				default: return false;
			}
		}

		template <glm::length_t L, glm::qualifier Q>
		bool fpvec_nonzero(const _fpvecNQ<L,Q>& v)
		{
			return glm::dot(v,v) != _ZERO;
		}

        fpscalar map01(const fpscalar v, const fpscalar n0, const fpscalar n1)
        {
			return (v-n0)/(n1-n0);
		}

		template <glm::length_t L, glm::qualifier Q>
	    _fpvecNQ<L,Q> map01(const _fpvecNQ<L,Q>& v, const _fpvecNQ<L,Q>& n0, const _fpvecNQ<L,Q>& n1)
		{
			return (v-n0)/(n1-n0);
		}


        uint8_t fp2u8(const fpscalar norm)
        {
        	return (norm > _ZERO ? (norm < _ONE ? uint8_t(round(norm*_FP255)) : uint8_t(255U)) : uint8_t(0U));
        }
        template <glm::length_t L, glm::qualifier Q>
        _u8vecNQ<L,Q> fp2u8(const _fpvecNQ<L,Q>& norm)
        {
        	//return _u8vecNQ<L,Q>(glm::round(glm::clamp(norm, _ZERO, _ONE)*_FP255));
        	//return _u8vecNQ<L,Q>(glm::clamp(glm::vec<L,int,Q>(glm::round(norm*_FP255)), glm::vec<L,int,Q>(0), glm::vec<L,int,Q>(255)));
        	return _u8vecNQ<L,Q>(glm::clamp(norm * 255.0, 0.0, 255.0));
        }

        fpscalar u8tofp(const uint8_t v)
        {
        	return _FPDIV255*v;
        }
        template <glm::length_t L, glm::qualifier Q>
        _fpvecNQ<L,Q> u8tofp(const _u8vecNQ<L,Q>& v)
        {
        	return _fpvecNQ<L,Q>(v)*_FPDIV255;
        }

        template <class IT, class OT=IT, class AT=OT>
        OT blerp(const IT v00, const IT v10, const IT v01, const IT v11, const fpvec2& factors)
        {
        	return OT(glm::mix(
        	    glm::mix(AT(v00), AT(v10), factors.x),
        	    glm::mix(AT(v10), AT(v11), factors.x),
        	    factors.y
        	));
        }

        uint8_t u8lerp(const uint8_t src, const uint8_t dst, const uint8_t alpha)
        {
            if (src == dst){return src;}
            const int srci = src, dsti = dst, alphai = alpha;
        	return uint8_t(glm::clamp((srci*(255-alphai) + dsti*alphai)/255, 0, 255));
        }
        template <glm::length_t L, glm::qualifier Q>
        _u8vecNQ<L,Q> u8lerp(const _u8vecNQ<L,Q>& src, const _u8vecNQ<L,Q>& dst, const uint8_t alpha)
        {
        	//const _uivecNQ<L,Q> _src(src);
        	//const _uivecNQ<L,Q> _dst(dst);
        	//return _u8vecNQ<L,Q>((_src*(255U-alpha) + _dst*uint(alpha))/255U);
        	_u8vecNQ<L,Q> res;
        	for (glm::length_t i = 0; i < L; i++){res[i] = u8lerp(src[i], dst[i], alpha);}
        	return res;
        }

        template <glm::length_t L, glm::qualifier Q>
        _u8vecNQ<L,Q> u8lerp(const _u8vecNQ<L,Q>& src, const _u8vecNQ<L,Q>& dst, const _u8vecNQ<L,Q>& alpha)
        {
            /*
        	const _uivecNQ<L,Q> _src(src);
        	const _uivecNQ<L,Q> _dst(dst);
        	const _uivecNQ<L,Q> _alpha(alpha);
        	const _uivecNQ<L,Q> _one(255U);
        	return _u8vecNQ<L,Q>((_src*(_one-_alpha) + _dst*_alpha)/_one);
        	*/
        	_u8vecNQ<L,Q> res;
        	for (glm::length_t i = 0; i < L; i++){res[i] = u8lerp(src[i], dst[i], alpha[i]);}
        	return res;
        }

        uint8_t u8afmul(const uint8_t v, const uint8_t alpha)
        {
        	return uint8_t(glm::clamp((int(v)*int(alpha) / 255), 0, 255));
        }
        template <glm::length_t L, glm::qualifier Q>
        _u8vecNQ<L,Q> u8afmul(const _u8vecNQ<L,Q>& v, const uint8_t alpha)
        {
            //const _uivecNQ<L,Q> _v(v);
        	//return ((_v * uint(alpha))/255U);
        	return _u8vecNQ<L,Q>(glm::clamp((_ivecNQ<L,Q>(v) * int(alpha))/255, 0, 255));
        }

        template <glm::length_t L, glm::qualifier Q>
        _u8vecNQ<L,Q> u8afmul(const _u8vecNQ<L,Q>& v, const _u8vecNQ<L,Q>& alpha)
        {
            //const _uivecNQ<L,Q> _v(v);
            //const _uivecNQ<L,Q> _a(alpha);
        	//return ((_v * _a)/255U);
        	return _u8vecNQ<L,Q>(glm::clamp((_ivecNQ<L,Q>(v) * _ivecNQ<L,Q>(alpha))/255, 0, 255));
        }
        /*
        template <glm::length_t L, glm::qualifier Q>
        _u8vecNQ<
        */
        uint8_t rgb2gray(const u8rgb& rgb)
        {
			return
			    u8afmul(rgb[0], _LUM_WEIGHTS_U8[0]) +
			    u8afmul(rgb[1], _LUM_WEIGHTS_U8[1]) +
			    u8afmul(rgb[2], _LUM_WEIGHTS_U8[2])
			;
		}
		fpscalar rgb2gray(const fpvec3& rgb)
		{
			return glm::dot(rgb, _LUM_WEIGHTS_FP);
		}
        template <glm::length_t L, glm::qualifier Q>
        _u8vecNQ<L,Q> u8blerp(const _u8vecNQ<L,Q>& v00, const _u8vecNQ<L,Q>& v10, const _u8vecNQ<L,Q>& v01, const _u8vecNQ<L,Q>& v11, const u8ra& factors)
        {
        	return u8lerp(
        	    u8lerp(v00, v10, factors.x),
        	    u8lerp(v10, v11, factors.x),
        	    factors.y
        	);
        }
        uint8_t u8blerp(const uint8_t v00, const uint8_t v10, const uint8_t v01, const uint8_t v11, const u8ra& factors)
        {
            return u8lerp(
        	   u8lerp(v00, v10, factors.x),
        	   u8lerp(v10, v11, factors.x),
        	   factors.y
        	);
        }
        int texcoord_wrap(const enum_t wraptype, const int c, const int s)
        {
            const int abs_s = glm::abs(s);
            if (abs_s == 0){return 0;}
        	switch (wraptype)
        	{
        		case enum_t::CLAMP_TO_EDGE:
        		    return glm::clamp(c, 0, abs_s-1);
        		default:
        		    return ((c % abs_s) + abs_s) % abs_s;
        	}
        }

        template<glm::length_t L, glm::qualifier Q>
        _u8vecNQ<L,Q> u8blend(const enum_t blendfunc, const _u8vecNQ<L,Q>& src, const _u8vecNQ<L,Q>& dst, const _u8vecNQ<L,Q>& factors)
        {
			using _u8vec = _u8vecNQ<L,Q>;
			using _uivec = _uivecNQ<L,Q>;
			using _ivec = _ivecNQ<L,Q>;
			/*
			switch (blendfunc)
			{
				case enum_t::BLEND_ADD: return _u8vec(glm::clamp(_ivec(dst) + _ivec(_math::_u8afmul(src, factors)), 0, 255));
				case enum_t::BLEND_SUB: return _u8vec(glm::clamp(_ivec(_math::_u8afmul(src, factors)) - _ivec(dst), 0, 255));
				case enum_t::BLEND_MUL: return _u8vec(glm::clamp(_ivec(dst) *_ivec(_math::_u8afmul(src, factors)), 0, 255));
				case enum_t::BLEND_DIV: return _u8vec(_math::_fp
				default: return _math::u8lerp(src, dst, factors);
			}
			*/
			switch (blendfunc)
			{
				case enum_t::BLEND_ADD: return _u8vec(glm::clamp(_ivec(src) + _ivec(_math::u8afmul(dst, factors)), 0, 255));
				case enum_t::BLEND_SUB: return _u8vec(glm::clamp(_ivec(src) - _ivec(_math::u8afmul(dst, factors)), 0, 255));
				case enum_t::BLEND_MUL: return _math::u8afmul(src, _math::u8afmul(dst, factors));
				case enum_t::BLEND_DIV: return src/glm::clamp(dst, uint8_t(1U), uint8_t(255U));
				default: return _math::u8lerp(src, dst, factors);
			}
		}
        template <glm::length_t L, glm::qualifier Q>
        #define _fpvecLQ glm::vec<L,fpscalar,Q>
        _fpvecLQ blend(const enum_t blendfunc, const _fpvecLQ& src, const _fpvecLQ& dst, const _fpvecLQ& factors) {
            switch (blendfunc) {
                case enum_t::BLEND_ADD: return src + (dst * factors);
                case enum_t::BLEND_SUB: return src - (dst * factors);
                case enum_t::BLEND_MUL: return src * (dst * factors);
                case enum_t::BLEND_DIV: return src / (dst * factors);
                default: return glm::mix(src, dst, factors);
            }
        }
        #undef _fpvecLQ

        fpvec4 wdiv(const fpvec4& pv)
        {
            const fpscalar w = pv.w;
        	return fpvec4(fpvec3(pv)/w, w);
        }

        fpvec3 perspective_divide(const fpvec3& affine, const fpvec3& invwvals)
        {
        	fpvec3 res = affine * invwvals;
        	res /= glm::dot(res, fpvec3(1));
        	return res;
        }
        ivec2 ndc2xy(const fpvec4& ndc, const int width, const int height)
        {
            const fpvec2 s = {width - 1, height - 1};
        	return ivec2(glm::round(((fpvec2(ndc) * 0.5) + 0.5) * s));
        	//return ivec2(s * (fpvec2(ndc)/ndc.w));
        }
        fpvec3 barycentric_calcweights(const fpmat2& bcm, const ivec2& xy, const ivec2& r3)
        {
        	const fpvec2 uv = bcm * fpvec2(xy-r3);
        	return fpvec3(uv, 1.0 - uv.x - uv.y);
        }
        struct tri2d_interpolator {
            private:
                fpmat2 bcmatrix;
                fpvec3 wvalues, invwvalues;
                fpvec3 affine_bcws, pc_bcws;
                fpvec3 bcws_zerovals, bcws_onevals;
                bool using_alt_bcwrange = false;
                u8rgb u8_affine_bcws, u8_pc_bcws;
                ivec2 r1, r2, r3;
            public:
                inline aabb2Di init(const aabb2Di& screenbounds, const fpvec4& v1_ndc, const fpvec4& v2_ndc, const fpvec4& v3_ndc, const bool remap_bcws=false, const fpvec3 bcws_rm0s={0,0,0}, const fpvec3 bcws_rm1s={1,1,1})
                {
                    const ivec2 sbdelta = screenbounds[1]-screenbounds[0];
                    const fpvec2 sbdeltaf(sbdelta);
                    const fpvec2& half = _HALF_FPV2;

                    //#define _ndc2xy(_ndc) ivec2(screenbounds[0].x + int(round(sbdeltaf.x * (0.5 + (_ndc.x * 0.5)))), screenbounds[1].y - int(round(sbdeltaf.y * (0.5 + (_ndc.y * 0.5)))))
                    #define _ndc2xy(_ndc) screenbounds[0]+ivec2(glm::round(sbdeltaf * (half + (fpvec2(_ndc) * half))))
                    this->r1 = _ndc2xy(v1_ndc);
                    this->r2 = _ndc2xy(v2_ndc);
                    this->r3 = _ndc2xy(v3_ndc);
                    #undef _ndc2xy

                	const aabb2Di bounds = {
                	    glm::max(glm::min(this->r1, glm::min(this->r2, this->r3)), screenbounds[0]),
                	    glm::min(glm::max(this->r1, glm::max(this->r2, this->r3)), screenbounds[1])
                	};
                	this->bcmatrix = glm::inverse(fpmat2{
                	    fpvec2(this->r1 - this->r3), fpvec2(this->r2 - this->r3)
                	});
                	this->wvalues = fpvec3{v1_ndc.w, v2_ndc.w, v3_ndc.w};
                	this->invwvalues = fpvec3{_ONE/(v1_ndc.w), _ONE/(v2_ndc.w), _ONE/(v3_ndc.w)};
                	this->using_alt_bcwrange = remap_bcws;
                	this->bcws_zerovals = bcws_rm0s;
                	this->bcws_onevals = bcws_rm1s;
                	this->recalc_weights(screenbounds[0].x, screenbounds[0].y);
                	return bounds;
                }
                inline bool recalc_weights(const int x, const int y)
                {
                    const ivec2 co = {x, y};
                    const ivec2& r3 = this->r3;
                	this->affine_bcws = fpvec3((this->bcmatrix)*fpvec2(co-r3), 0.0);
                	this->affine_bcws.z = fpscalar(1) - (this->affine_bcws.x) - (this->affine_bcws.y);
                	if (this->using_alt_bcwrange)
                	{
						this->affine_bcws = glm::mix(this->bcws_zerovals, this->bcws_onevals, this->affine_bcws);
					}
                	if (glm::any(glm::lessThan(this->affine_bcws, _ZERO_FPV3))){return false;}

                	this->pc_bcws = (this->affine_bcws)*(this->invwvalues);
                	this->pc_bcws /= (this->pc_bcws.x + this->pc_bcws.y + this->pc_bcws.z);

                	this->u8_affine_bcws = fp2u8(this->affine_bcws);
                	this->u8_pc_bcws = fp2u8(this->pc_bcws);
                	return true;
                }

                template <class IT, class OT=IT, class AT=OT>
                inline OT terp(const IT ra1, const IT ra2, const IT ra3, const bool perspective_correct=false) const
                {
                	const fpvec3& uvw = (perspective_correct ? this->pc_bcws : this->affine_bcws);
                	return OT( (AT(ra1)*uvw.x) + (AT(ra2)*uvw.y) + (AT(ra3)*uvw.z) );
                }
                template <class IT, class OT=IT, class AT=OT>
                inline OT terp(const IT* rvals, const bool perspective_correct=false) const
                {
                	const fpvec3& uvw = (perspective_correct ? this->pc_bcws : this->affine_bcws);
                	return OT( (AT(rvals[0])*uvw.x) + (AT(rvals[1])*uvw.y) + (AT(rvals[2])*uvw.z) );
                }
                inline uint8_t u8terp(const uint8_t ra1, const uint8_t ra2, const uint8_t ra3, const bool perspective_correct=false) const
                {
                	const u8rgb& uvw = (perspective_correct ? this->u8_pc_bcws : this->u8_affine_bcws);
                	return uint8_t(glm::clamp(uint(u8afmul(ra1, uvw.x)) + uint(u8afmul(ra2, uvw.y)) + uint(u8afmul(ra3, uvw.z)), 0U, 255U));
                }
                template <glm::length_t L, glm::qualifier Q>
                inline _u8vecNQ<L,Q> u8terp(const _u8vecNQ<L,Q>& ra1, const _u8vecNQ<L,Q>& ra2, const _u8vecNQ<L,Q>& ra3, const bool perspective_correct=false) const
                {
                	const u8rgb& uvw = (perspective_correct ? this->u8_pc_bcws : this->u8_affine_bcws);
                	return _u8vecNQ<L,Q>(glm::clamp(_uivecNQ<L,Q>(u8afmul(ra1, uvw.x)) + _uivecNQ<L,Q>(u8afmul(ra2, uvw.y)) + _uivecNQ<L,Q>(u8afmul(ra3, uvw.z)), 0U, 255U));
                }

                template <class U8T=uint8_t>
                inline U8T u8terp(const U8T* rvals, const bool perspective_correct=false) const
                {
                	//const u8rgb& uvw = (perspective_correct ? this->u8_pc_bcws : this->u8_affine_bcws);
                	//return u8afmul(rvals[0], uvw.x) + u8afmul(rvals[1], uvw.y) + u8afmul(rvals[2], uvw.z);
                	return this->u8terp(rvals[0], rvals[1], rvals[2], perspective_correct);
                }

        };

    }
    inline void set_color_ptr(uint8_t* dst, const u8rgba& src, const enum_t dstfmt)
    {
        const u8rgb& lumws = _math::_LUM_WEIGHTS_U8;
    	switch (dstfmt)
    	{
    		case enum_t::GRAYSCALE: *dst = (_math::u8afmul(src.x, lumws.x) + _math::u8afmul(src.y, lumws.y) + _math::u8afmul(src.z, lumws.z)); break;
    		case enum_t::GRAYALPHA: dst[0] = (_math::u8afmul(src.x, lumws.x) + _math::u8afmul(src.y, lumws.y) + _math::u8afmul(src.z, lumws.z)); dst[1] = src.w; break;
    		case enum_t::RGB: memcpy(dst, &src, 3); break;
    		case enum_t::RGBA: memcpy(dst, &src, 4); break;
    		default: return;
    	}
    }
    inline u8rgba get_color_fromptr(const uint8_t* src, const enum_t srcfmt)
    {
    	if (src == nullptr){return u8rgba(0);}
    	switch (srcfmt)
    	{
    		case enum_t::GRAYSCALE: return u8rgba(u8rgb(*src), 255U);
    		case enum_t::GRAYALPHA: return u8rgba(u8rgb(src[0]), src[1]);
    		case enum_t::RGB: return u8rgba(*(const u8rgb*)src, 255U);
    		case enum_t::RGBA: return *(const u8rgba*)src;
    		case enum_t::DEPTH_COMPONENT: return u8rgba(u8rgb(_math::fp2u8(*(const fpscalar*)src)), 255U);
    		default: return u8rgba(0);
    	}
    }
    struct Framebuffer : public _ContextManageableObject {
        public:
            int width, height, rbuf_nch;
            enum_t rbuf_fmt;
            bool _owns_rbuf, _owns_dbuf;
            uint8_t* renderbuffer = nullptr;
            fpscalar* depthbuffer = nullptr;
            inline size_t _xy2idx(const ivec2& xy)
            {
            	return ((this->height-1-xy.y)*this->width)+xy.x;
            }
            void init(const int w, const int h, const enum_t rbf=enum_t::RGB, uint8_t* rbuf=nullptr, fpscalar* dbuf=nullptr)
            {
            	this->width = w;
            	this->height = h;
            	this->rbuf_fmt = rbf;
            	this->rbuf_nch = int(rbf);

            	if (this->_owns_rbuf = ((this->renderbuffer = rbuf) == nullptr)){this->renderbuffer = new uint8_t[w*h*int(rbf)]; memset(this->renderbuffer, 0, w*h*int(rbf));}
            	if (this->_owns_dbuf = ((this->depthbuffer = dbuf) == nullptr)){this->depthbuffer = new fpscalar[w*h]; std::fill_n(this->depthbuffer, w*h, 1.0);}
            }
            void close()
            {
                if (this->_owns_rbuf && (this->renderbuffer != nullptr))
                {
                	delete[] this->renderbuffer;
                }
                if (this->_owns_dbuf && (this->depthbuffer != nullptr))
                {
                	delete[] this->depthbuffer;
                }
                this->renderbuffer = nullptr;
                this->depthbuffer = nullptr;
            }
            inline uint8_t* colorptr(const int x, const int y)
            {
            	if (x < 0 || x >= this->width || y < 0 || y >= this->height){return nullptr;}
                return (this->renderbuffer+(this->_xy2idx(ivec2{x,y})*this->rbuf_nch));
            }
            inline fpscalar* depthptr(const int x, const int y)
            {
            	if (x < 0 || x >= this->width || y < 0 || y >= this->height){return nullptr;}
            	return (this->depthbuffer+(this->_xy2idx(ivec2{x,y})));
            }

            //Framebuffer() {}
            //~Framebuffer() {this->close();}
            void on_destroy() {this->close();}

    };
    struct _TextureMipLevel {
        int mipwidth = 0, mipheight = 0;
        bool _owns_miptexels = false;
        uint8_t* miptexels = nullptr;
        bool origin_tl = false;
    };
    struct Texture : public _ContextManageableObject {
        int width, height;
        size_t pxsz;
        int num_miplevels = 0;
        bool _owns_texels = false;
        uint8_t* texels = nullptr;
        _TextureMipLevel mipmaps[DJ_SW3D20_MAX_TEXTURE_MIPMAPS];
        enum_t texelfmt = enum_t::RGB, filter = enum_t::NEAREST, wrap_s = enum_t::REPEAT, wrap_t = enum_t::REPEAT;
        inline void* _xy2ptr(const ivec2& xy, const int level=0)
        {
        	if (level < 0 || level >= this->num_miplevels){return nullptr;}
        	_TextureMipLevel& mipmap = this->mipmaps[level];
        	//const int x = _math::texcoord_wrap(this->wrap_s, xy.x, mipmap.mipwidth);
        	//const int y = mipmap.mipheight - 1 - _math::texcoord_wrap(this->wrap_t, (xy.y), mipmap.mipheight);
        	//const int y = _math::texcoord_wrap(this->wrap_t, (mipmap.mipheight-1-xy.y), mipmap.mipheight);
            const int x = glm::clamp(xy.x, 0, mipmap.mipwidth-1);
            const int y = (mipmap.origin_tl ? glm::clamp(xy.y, 0, mipmap.mipheight-1) : mipmap.mipheight - 1 - glm::clamp(xy.y, 0, mipmap.mipheight-1));
            return mipmap.miptexels + (((y * mipmap.mipwidth) + x) * this->pxsz);
        }
        inline u8rgba _xy2color(const ivec2& xy, const int level=0)
        {
        	return get_color_fromptr((uint8_t*)this->_xy2ptr(xy, level), this->texelfmt);
        }
        fpscalar _tc_wrapaxis(const fpscalar& in, const enum_t& w) {
            switch (w) {
                case enum_t::CLAMP_TO_EDGE: return glm::clamp(in);
                //default: return glm::repeat(in);
                default: return glm::mod(in, fpscalar(1));
            }
        }
        inline u8rgba _lsample2D(const int level, const enum_t lfilter, const fpvec2& st)
        {
            if (level < 0 || level >= this->num_miplevels){return u8rgba(0);}
            _TextureMipLevel& mipmap = this->mipmaps[level];
            const fpvec2 uv = {
                this->_tc_wrapaxis(st.x, this->wrap_s),
                this->_tc_wrapaxis(st.y, this->wrap_t)
            };
            const fpvec2 xyf = uv*fpvec2{mipmap.mipwidth-1, mipmap.mipheight-1};

        	switch (lfilter)
        	{
        		case enum_t::LINEAR:
        		{
        			const ivec2 blc = ivec2(xyf), brc = blc+ivec2{1, 0}, tlc = blc+ivec2{0, 1}, trc = blc+ivec2{1, 1};
        			const fpvec2 blfacs_f = xyf-fpvec2(blc);
        			//const u8ra blfacs = _math::fp2u8(blfacs_f);
        			return u8rgba(glm::clamp(glm::mix(
        				glm::mix(fpvec4(this->_xy2color(blc, level)), fpvec4(this->_xy2color(brc, level)), blfacs_f.x),
        				glm::mix(fpvec4(this->_xy2color(tlc, level)), fpvec4(this->_xy2color(trc, level)), blfacs_f.x),
        				blfacs_f.y
        			), 0.0, 255.0));
        		}
        		default:
        		{
        			return this->_xy2color(ivec2(glm::round(xyf)), level);
        		}
        	}

        }
        void _clearmip(const int level)
        {
        	_TextureMipLevel& cmip = this->mipmaps[level];
      		if (cmip._owns_miptexels)
      		{
      			delete[] cmip.miptexels;
      		}
      		cmip.miptexels = nullptr;
      		cmip.mipwidth = cmip.mipheight = 0;
      		cmip._owns_miptexels = false;
        }
        void clear_mipmaps()
        {
        	for (int i = 0; i < this->num_miplevels; i++)
        	{
        		this->_clearmip(i);
        	}
        	this->num_miplevels = 0;
        }
        void regen_mipmaps(const int maxlevels=DJ_SW3D20_MAX_TEXTURE_MIPMAPS)
        {
        	this->clear_mipmaps();
            int curwidth = this->width, curheight = this->height;
            //const fpvec2 mm0incrs = {1.0/(curwidth), 1.0/(curheight)};
            this->num_miplevels = 0;
            for (int lvl = 0; (lvl < maxlevels) && (lvl < DJ_SW3D20_MAX_TEXTURE_MIPMAPS); lvl++, this->num_miplevels++)
            {
                curwidth = this->width / (1 << lvl);
                curheight = this->height / (1 << lvl);
            	//std::cout << "\nlevel: " << lvl << ", size: " << curwidth << 'x' << curheight << '\n';
            	_TextureMipLevel& mipmap = this->mipmaps[lvl];
            	mipmap.mipwidth = curwidth;
            	mipmap.mipheight = curheight;
            	if (lvl == 0)
            	{
            		mipmap._owns_miptexels = false;
            		mipmap.miptexels = this->texels;
            	}
            	else if ((lvl > 0) && (curwidth > 0) && (curheight > 0) && (curwidth % 2 == 0) && (curheight % 2 == 0))
            	{
            	    /*
            		mipmap._owns_miptexels = true;
            		mipmap.miptexels = new uint8_t[mipmap.mipwidth*mipmap.mipheight*this->pxsz];
            		_TextureMipLevel& prevmip = this->mipmaps[lvl-1];
            		//int ox = 0, oy = 0;
            		ivec3 oxy, nxy;
            		for (nxy.x = 0; nxy.x < curwidth; nxy.x++)
            		{
            			oxy.x = glm::round((prevmip.mipwidth-1)*(nxy.x / fpscalar(curwidth-1)));
            			for (nxy.y = 0; nxy.y < curheight; nxy.y++)
            			{
            				oxy.y = glm::round((prevmip.mipheight-1)*(nxy.y / fpscalar(curheight-1)));
            				memcpy(this->_xy2ptr(nxy, lvl), this->_xy2ptr(oxy, lvl-1), this->pxsz);
            			}
            		}
            		*/
            		mipmap._owns_miptexels = true;
            		mipmap.miptexels = new uint8_t[curwidth*curheight*this->pxsz];
            		ivec2 xy;
            		const fpvec2 xyincrs = {(this->width)/fpscalar(curwidth), (this->height)/fpscalar(curheight)};
            		for (xy.y = 0; xy.y < curheight; xy.y++)
            		{
            			for (xy.x = 0; xy.x < curwidth; xy.x++)
            			{
            				const ivec2 oxy = ivec2((xyincrs*fpvec2(xy)));
            				memcpy(mipmap.miptexels + ((((curheight - 1 - xy.y) * curwidth)+xy.x)*this->pxsz), this->_xy2ptr(oxy, 0), this->pxsz);
            			}
            		}

            	}
            	else
            	{
            	    //std::cout << "num_miplevels = " << this->num_miplevels << '\n';
            	    //this->num_miplevels++;
            		break;
            	}
            }
        }
        void close()
        {
        	this->clear_mipmaps();
        	if (this->_owns_texels && this->texels != nullptr)
        	{
        	    delete[] this->texels;
        	}
        	this->_owns_texels = false;
        	this->texels = nullptr;
        }
        void texImage2D(void* pixels, const int w, const int h, const enum_t tfmt=enum_t::RGB, const int level=0, const bool copy=false)
        {
            if (level <= 0)
            {
            	this->close();
            	this->width = w;
            	this->height = h;
            	this->texelfmt = tfmt;
            	switch (tfmt)
            	{
            		case enum_t::GRAYSCALE: this->pxsz = 1; break;
            		case enum_t::GRAYALPHA: this->pxsz = 2; break;
            		case enum_t::RGB: this->pxsz = 3; break;
            		case enum_t::RGBA: this->pxsz = 4; break;
            		case enum_t::DEPTH_COMPONENT: this->pxsz = sizeof(fpscalar); break;
            		default:
            		    throw std::runtime_error("invalid texelfmt!\n");
            	}
            	if (copy || (pixels == nullptr))
            	{
            		this->_owns_texels = true;
            		this->texels = new uint8_t[w*h*this->pxsz];
            		if (copy && (pixels != nullptr)){memcpy(this->texels, pixels, w*h*this->pxsz);}
            	}
            	else
            	{
            	    this->_owns_texels = false;
            		this->texels = (uint8_t*)pixels;
            	}
            	this->regen_mipmaps(1);
            }
            else if (level > 0 && level < DJ_SW3D20_MAX_TEXTURE_MIPMAPS)
            {
            	if (level == this->num_miplevels)
            	{
            		this->num_miplevels++;
                }
           		this->_clearmip(level);
           		_TextureMipLevel& mip = this->mipmaps[level];
           		mip.mipwidth = w;
           		mip.mipheight = h;
           		if (copy || (pixels == nullptr))
           		{
           			mip._owns_miptexels = true;
           			mip.miptexels = new uint8_t[w*h*this->pxsz];
           			if (copy && (pixels != nullptr)){memcpy(mip.miptexels, pixels, w*h*this->pxsz);}
           		}
           		else
           		{
           			mip._owns_miptexels = false;
           			mip.miptexels = (uint8_t*)pixels;
           		}
            }
            else
            {
            	throw std::runtime_error("bad level!\n");
            }
        }

        inline u8rgba sample2D(const fpvec2& uv, const fpscalar lodfactor=0.0)
        {
            //const bool lvls_same = (toplvl == btmlvl);
            const fpscalar lod = glm::clamp(lodfactor, 0.0, 1.0)*(this->num_miplevels-1);
            //std::cout << "lod = " << lod << '\n';
            const int toplvl = int(lod), btmlvl = glm::min(toplvl+1, this->num_miplevels-1);
            //std::cout << "lod = " << lod << ", top = " << toplvl << ", bottom = " << btmlvl << '\n';
            const fpscalar lodw = lod-toplvl;
        	switch (this->filter)
        	{
        		case enum_t::NEAREST: return this->_lsample2D(0, enum_t::NEAREST, uv);
        		case enum_t::LINEAR: return this->_lsample2D(0, enum_t::LINEAR, uv);
        		case enum_t::NEAREST_MIPMAP_NEAREST:
        		{
        			return this->_lsample2D(int(round(lod)), enum_t::NEAREST, uv);
        		}
        		case enum_t::NEAREST_MIPMAP_LINEAR:
	       		{
	       			return _math::u8lerp(this->_lsample2D(toplvl, enum_t::NEAREST, uv), this->_lsample2D(btmlvl, enum_t::NEAREST, uv), _math::fp2u8(lodw));
	       			//return u8rgba(0);
	       		}
                case enum_t::LINEAR_MIPMAP_NEAREST:
        		{
        			return this->_lsample2D(int(round(lod)), enum_t::LINEAR, uv);
        		}
        		case enum_t::LINEAR_MIPMAP_LINEAR:
	       		{
	       			return _math::u8lerp(this->_lsample2D(toplvl, enum_t::LINEAR, uv), this->_lsample2D(btmlvl, enum_t::LINEAR, uv), _math::fp2u8(lodw));
	       			//return u8rgba(0);
	       		}
        		default: return u8rgba(0);
        	}
        }
        //Texture() {};
        //~Texture() {this->close();}
        void on_destroy() {this->close();}
    };
    struct EnvironmentMap : public _ContextManageableObject {
        private:
            Texture targets[7] = {};
        public:
            Texture* operator[](const enum_t envtg) const {
                const int index = int(envtg);
            	if (index < 0 || index >= sizeof(this->targets)/sizeof(this->targets[0])){return nullptr;}
            	return (Texture*)(this->targets + index);
            }
            inline u8rgba sampleEquirect(const fpvec3& vector, const fpscalar lodfactor=0.0)
            {
                return this->targets[0].sample2D(_math::equirect2uv(vector), lodfactor);
            }
            inline u8rgba sampleCube(const fpvec3& vector, const fpscalar lodfactor=0.0){
                enum_t face; fpvec2 faceuv;
                _math::vec2cubemap(vector, face, faceuv);
                //faceuv = glm::clamp(faceuv, fpvec2(0), fpvec2(1));
                return this->targets[int(face)].sample2D(faceuv, lodfactor);
            }
            void on_create()
            {
            	for (int i = 0; i < sizeof(this->targets)/sizeof(this->targets[0]); i++)
            	{
            		auto& tg = this->targets[i];
            		tg.filter = enum_t::NEAREST;
            		tg.wrap_s = tg.wrap_t = enum_t::CLAMP_TO_EDGE;
            	}
            }

    };
    template <class T>
    struct _MaterialTexAffect {
		bool use = false;
		T amount = T(0.0);
	};
    struct MaterialTextureSlot {
		bool use = true;
		enum_t textype = enum_t::TEXTYPE_IMAGE;
		enum_t envmaptype = enum_t::ENVMAP_EQUIRECT;
		Texture* src_image = nullptr;
		EnvironmentMap* src_envmap = nullptr;

		_MaterialTexAffect<fpvec3> affects_diffuse_color = {true, fpvec3(1.0)};
		_MaterialTexAffect<fpvec3> affects_specular_color = {false, fpvec3(1.0)};
		_MaterialTexAffect<fpvec3> affects_emit_color = {false, fpvec3(1.0)};
		//fpvec3
		    //affects_diffuse_color = fpvec3(1.0),
		    //affects_specular_color = fpvec3(0.0),
		    //affects_emit_color = fpvec3(0.0)
		//;
		/*
		fpscalar
		    affects_diffuse_alpha = 0.0,
		    affects_specular_alpha = 0.0,
		    affects_diffuse_intensity = 0.0,
		    affects_specular_intensity = 0.0,
		    affects_specular_hardness = 0.0,
		    affects_emit_amount = 0.0
		;
		*/
		_MaterialTexAffect<fpscalar> affects_diffuse_alpha = {false, 0.0};
		_MaterialTexAffect<fpscalar> affects_specular_alpha = {false, 0.0};
		_MaterialTexAffect<fpscalar> affects_diffuse_intensity = {false, 0.0};
		_MaterialTexAffect<fpscalar> affects_specular_intensity = {false, 0.0};
		_MaterialTexAffect<fpscalar> affects_specular_hardness = {false, 0.0};
		_MaterialTexAffect<fpscalar> affects_emit_amount = {false, 0.0};
		_MaterialTexAffect<fpvec3> affects_texcoord_offset = {false, fpvec3(0.0)};
		_MaterialTexAffect<fpvec3> affects_normal_vector = {false, fpvec3(0.0)};


		fpvec3 texcoord_offset = {0.0,0.0,0.0}, texcoord_scaling = {1.0,1.0,1.0};
		enum_t mapping_type = enum_t::TEXMAP_UVMAP;
		int uvmapno = 0;
		int alpha_channel = 3;
		enum_t layerblendtype = enum_t::BLEND_MIX;
		enum_t bumpspace = enum_t::BUMPMAPSPACE_OBJECT;
	};
	using texslotlist_t = std::vector<MaterialTextureSlot>;
    struct Material {
		fpvec4
		    base_diffuse_color = {0.75,0.75,0.75, 1.0},
		    base_specular_color = {1,1,1, 1}
		;
		fpscalar
		    base_diffuse_intensity = 1.0,
		    base_specular_intensity = 0.0,
		    base_specular_hardness = 0.1
		;
     	_MaterialTexAffect<fpvec4> vertex_color_paint = {false, fpvec4(1.0)};
     	_MaterialTexAffect<fpvec4> vertex_color_light = {false, fpvec4(1.0)};
		enum_t vertex_color_light_blend = enum_t::BLEND_MUL;
		bool shadeless = true;
		bool uses_transparency = false;
		enum_t blendtype = enum_t::BLEND_MIX;
		//MaterialTextureSlot texture_slots[DJ_SW3D20_MATERIAL_MAX_TEXSLOTS];
		texslotlist_t texture_slots;

		fpscalar alpha_threshold = 0;

	};
    enum struct light_type : int {
		point = 0,
		spot = 1,
		ambient = 2,
		hemi = 3
	};
	enum struct light_falloff_model : int {
		constant = 0,
		linear = 1,
		square = 2
	};
    struct LightSource {
		bool active = true;
		light_type type = light_type::point;
		light_falloff_model falloff = light_falloff_model::linear;

		fpvec3 pos = {0,0,0}, scale = {1,1,1};
		fpmat3 orientation = fpmat3(1.0);

		fpscalar point_radius_max = 1.0, point_radius_min = 0.0;

		fpvec4 diffuse_color = {1,1,1,0}, specular_color = {1,1,1,1}, shadow_color = {0,0,0,0};

		fpscalar diffuse_intensity = 1.0, specular_intensity = 1.0;

		bool casts_shadows = false;
		fpscalar spot_cone_angle = 90.0, spot_max_distance = 1.0;
	};
	fpscalar _calc_light_factor(const light_falloff_model falloff, const fpscalar& distance, const fpscalar& radius_min, const fpscalar& radius_max) {
		if (falloff == light_falloff_model::constant){return fpscalar(1);}
		fpscalar basefac = glm::clamp((distance - radius_min) / (radius_max - radius_min), fpscalar(0), fpscalar(1));
		fpscalar factor;
	    switch (falloff) {
			case light_falloff_model::square: {
				factor = 1.0 - (basefac*basefac);
				break;
			}
			default: factor = 1.0 - basefac;
		}
		return factor;
	}

    struct ffvertex_t {
		fpvec3
		    co = {0,0,0},
		    normal = {0,0,1}
		;

		fpvec2 uv[16];

		fpvec4 color = {1,1,1,1};
		//fpvec3 tangent = {0,0,0}, binormal = {0,0,0};
	};

    using ffvertexlist_t = std::vector<ffvertex_t>;

    struct RenderPipelineSettings {
		bool
		    cap_face_culling = false,
		    cap_depth_test = false,
		    cap_draw_color_buffer[4] = {true, true, true, true},
		    cap_draw_depth_buffer = true,
			cap_blending = true
		;
		enum_t
		    en_cull_face = enum_t::BACK_FACE,
		    en_depth_func = enum_t::CMP_LESS,
		    en_draw_type = enum_t::DRAW_TRIANGLES,
		    en_matrix_mode = enum_t::PROJECTION_MATRIX
			//en_blend_func = enum_t::BLEND_MIX,
			//en_blend_alpha_src = enum_t::SRC_ALPHA
		;
		fpvec4 clear_color = fpvec4(0);
		fpscalar clear_depth = 1;
		inline bool* _valptr_b(const enum_t cap)
		{
			switch (cap)
			{
				case enum_t::CAP_FACE_CULLING: return &(this->cap_face_culling);
				case enum_t::CAP_DEPTH_TEST: return &(this->cap_depth_test);
				case enum_t::CAP_DRAW_COLOR_BUFFER_R: return &(this->cap_draw_color_buffer[0]);
				case enum_t::CAP_DRAW_COLOR_BUFFER_G: return &(this->cap_draw_color_buffer[1]);
				case enum_t::CAP_DRAW_COLOR_BUFFER_B: return &(this->cap_draw_color_buffer[2]);
				case enum_t::CAP_DRAW_COLOR_BUFFER_A: return &(this->cap_draw_color_buffer[3]);
				case enum_t::CAP_DRAW_DEPTH_BUFFER: return &(this->cap_draw_depth_buffer);
				case enum_t::CAP_BLENDING: return &(this->cap_blending);
				default: return nullptr;
			}
		}
	};
	//struct Context;
	/*
    struct RenderPipelineState {
		Context* ctx = nullptr;
		const int threadno = 0;
		ivec2 fragcoord_xy;
		fpvec4 fragcoord_ndc;
		fpvec3 frag_lerpweights, frag_lerpweights_pc;
	};
	*/
	template <class ST, glm::length_t C, glm::length_t R, glm::qualifier Q=glm::defaultp>
	struct _matstack {
		using matrix_type = glm::mat<C, R, ST, Q>;
		private:
		    std::vector<matrix_type> _ims;
		public:
		    size_t size() const {return this->_ims.size();}

			void srPushMatrix()
			{
				//const matrix_type cur = this->_ims.back();
				this->_ims.push_back(this->_ims.back());
			}

			bool srPopMatrix()
			{
				if (this->size() > 1)
				{
					this->_ims.pop_back();
					return true;
				}
				return false;
			}

			matrix_type& srTopMatrix()
			{
				return this->_ims[this->_ims.size() - 1];
			}

			void srMultMatrix(const matrix_type& m)
			{
				this->srTopMatrix() = m * this->srTopMatrix();
			}

			void srLoadIdentity()
			{
				this->srTopMatrix() = matrix_type(ST(1));
			}
			void srReloadParent()
			{
				if (this->size() > 1){this->srTopMatrix() = *(this->_ims.crbegin()+1);}
				else {this->srLoadIdentity();}
			}

			operator matrix_type() const {return matrix_type(this->_ims.back());}

			_matstack(const size_t num_reserve=64)
			{
				//this->_ims.reserve(num_reserve);
				this->_ims.push_back(matrix_type(ST(1)));
				std::cout << "matstack init size = " << this->_ims.size() << '\n';
			}
	};
	/*
	struct BitmapFont {

	};
	*/
	struct _affine_lerper {
	    enum_t curmode = enum_t::DRAW_TRIANGLES;
	    fpmat2 tri_bcmatrix;
	    fpvec3 wvals, invwvals;
	    ffvertex_t* in_verts[3];
	    fpvec4 ndc[3];
	    ivec2 screenverts[3];
	    aabb2Di drawbox, screenbox;
	    fpvec3 bl_bcw, br_bcw, tl_bcw, tr_bcw;

	    fpscalar bcw_xfincr, bcw_yfincr;
	    fpvec3 bcw_xdiff, bcw_yxl, bcw_yxr;
	    fpvec2 bcw_lerpfactors;
	    Framebuffer* curfbo = nullptr;
	    uint8_t* crbptr = nullptr;
	    fpscalar* cdbptr = nullptr;
	    fpvec3 bcw_remaps[2] = {{0,0,0}, {1,1,1}};
	    fpvec3 affine_bcws, pc_bcws;
	    u8rgb u8_affine_bcws, u8_pc_bcws;
	    ivec2 lcurpx, curpx;
	    _affine_lerper() {}
	    void setfbo(Framebuffer* fbo){
	        this->curfbo = fbo;
	        this->screenbox = aabb2Di{ivec2(0), ivec2{fbo->width - 1, fbo->height - 1}};
	        this->crbptr = fbo->renderbuffer;
	        this->cdbptr = fbo->depthbuffer;
	    }
	    inline ivec2 _ndc2xy(const fpvec4& _ndc)
	    {
	        const fpvec2 n = (fpvec2(_ndc) * 0.5) + 0.5;
	        const fpvec2 s = {this->curfbo->width-1, this->curfbo->height-1};
	    	return ivec2(glm::round(s*n));
	    }
	    bool _startrow()
	    {
	        if (this->curpx.y > this->drawbox[1].y){return false;}
	    	this->bcw_lerpfactors[0] = 0.0;
	    	this->bcw_lerpfactors[1] = (this->curpx.y - this->drawbox[0].y) * this->bcw_yfincr;

	    	this->curpx.x = this->drawbox[0].x;

	    	this->crbptr = this->curfbo->colorptr(this->curpx.x, this->curpx.y);
	    	this->cdbptr = this->curfbo->depthptr(this->curpx.x, this->curpx.y);

	    	this->bcw_yxl = glm::mix(this->bl_bcw, this->tl_bcw, this->bcw_lerpfactors[1]);
	    	this->bcw_yxr = glm::mix(this->br_bcw, this->tr_bcw, this->bcw_lerpfactors[1]);

	    	this->bcw_xdiff = this->bcw_yxr - this->bcw_yxl;

	    	return true;
	    }
	    bool next()
	    {

	    	if (this->curpx.x >= this->drawbox[1].x)
	    	{
	    		this->curpx.y++;
	    		if (!this->_startrow()){return false;}
	    		//this->lastpx = curpx;
	    	}
	    	else
	    	{
	    	    this->curpx.x++;
	    	    this->crbptr += this->curfbo->rbuf_nch;
	    	    this->cdbptr++;
	    	    this->bcw_lerpfactors[0] += this->bcw_xfincr;
	    	}
	    	return true;
	    }
	    bool calc(const bool skip_outside=true)
	    {
	    	const fpvec3 affine = glm::mix(this->bcw_yxl, this->bcw_yxr, this->bcw_lerpfactors[0]);
	    	if (skip_outside && glm::any(glm::lessThan(affine, _math::_ZERO_FPV3))){return false;}

	    	this->affine_bcws = glm::mix(this->bcw_remaps[0], this->bcw_remaps[1], affine);
	    	this->u8_affine_bcws = _math::fp2u8(this->affine_bcws);

	    	this->pc_bcws = this->affine_bcws * (this->invwvals);
	    	this->pc_bcws /= glm::dot(this->pc_bcws, fpvec3(1));
	    	this->u8_pc_bcws = _math::fp2u8(this->pc_bcws);

	    	return true;
	    }
	    inline bool cond()
	    {
	    	return this->curpx.y <= this->drawbox[1].y;
	    }
	    void start_triangle(const fpvec4& ndc1, const fpvec4& ndc2, const fpvec4& ndc3)
	    {
	        this->curmode = enum_t::DRAW_TRIANGLES;
	        this->screenverts[0] = this->_ndc2xy((this->ndc[0] = ndc1));
	        this->screenverts[1] = this->_ndc2xy((this->ndc[1] = ndc2));
	        this->screenverts[2] = this->_ndc2xy((this->ndc[2] = ndc3));

	        this->invwvals = 1.0/((this->wvals = fpvec3{ndc1.w, ndc2.w, ndc3.w}));

	        this->drawbox[0] = glm::clamp(glm::min(this->screenverts[0], glm::min(this->screenverts[1], this->screenverts[2])), this->screenbox[0], this->screenbox[1]);
	        this->drawbox[1] = glm::clamp(glm::max(this->screenverts[0], glm::max(this->screenverts[1], this->screenverts[2])), this->screenbox[0], this->screenbox[1]);
	        const ivec2 delta = this->drawbox[1] - this->drawbox[0];
	        this->curpx = this->drawbox[0];

	        fpvec3* bcwptrs[4] = {&(this->bl_bcw), &(this->br_bcw), &(this->tl_bcw), &(this->tr_bcw)};

	        this->tri_bcmatrix = glm::inverse(fpmat2{
	            fpvec2(this->screenverts[0] - this->screenverts[2]),
	            fpvec2(this->screenverts[1] - this->screenverts[2])
	        });

	        ivec2 cornmuls[4] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};

	        for (int i = 0; i < 4; i++) //calc for each corner
	        {
	        	const ivec2 co = this->drawbox[0]+(delta*cornmuls[i]);

	        	*(bcwptrs[i]) = fpvec3(this->tri_bcmatrix * fpvec2(co - this->screenverts[2]), 1.0);
	        	bcwptrs[i]->z = 1.0 - bcwptrs[i]->x - bcwptrs[i]->y;
	        }
	        this->bcw_xfincr = 1.0/(delta[0]);
	        this->bcw_yfincr = 1.0/(delta[1]);
	        this->bcw_lerpfactors = fpvec2(0);
	        this->_startrow();
	    }

	    fpscalar lerpf(const fpscalar v0, const fpscalar v1, const fpscalar v2, const bool perspective_correct=false)
	    {
	    	const fpvec3& weights = (perspective_correct ? this->pc_bcws : this->affine_bcws);
	    	return glm::dot(fpvec3{v0, v1, v2}, weights);
	    }
	    uint8_t lerpu8(const uint8_t v0, const uint8_t v1, const uint8_t v2, const bool perspective_correct=false)
	    {
	    	const uvec3 weights = uvec3(perspective_correct ? this->u8_pc_bcws : this->u8_affine_bcws);
	    	return uint8_t(glm::clamp(((weights.x * v0) + (weights.y * v1) + (weights.z * v2))/255U, 0U, 255U));
	    }
	    #define _fpvt _fpvecNQ<L,Q>
	    template <glm::length_t L, glm::qualifier Q>
	    _fpvt lerpf(const _fpvt& v0, const _fpvt& v1, const _fpvt& v2, const bool perspective_correct=false)
	    {
	    	const fpvec3& weights = (perspective_correct ? this->pc_bcws : this->affine_bcws);
	    	return (v0 * weights.x) + (v1 * weights.y) + (v2 * weights.z);
	    }
	    #undef _fpvt
	    #define _u8vt _u8vecNQ<L,Q>
	    #define _uivt _uivecNQ<L,Q>
	    template <glm::length_t L, glm::qualifier Q>
	    _u8vt lerpu8(const _u8vt& v0, const _u8vt& v1, const _u8vt& v2, const bool perspective_correct=false)
	    {
	        const uvec3 weights = uvec3(perspective_correct ? this->u8_pc_bcws : this->u8_affine_bcws);
	        const _uivt mul = ((_uivt(v0) * weights.x) + (_uivt(v1) * weights.y) + (_uivt(v2) * weights.z))/255U;
	        return _u8vt(glm::clamp(mul, 0U, 255U));
	    }
	    #undef _u8vt
	    #undef _uivt
	};

	struct _tri_barycentric_holder {
	    fpvec3 bcweights = {0,0,0};
	    ivec3 bcweights_u8i = {0,0,0};
	    inline fpscalar lerpF(const fpscalar v0, const fpscalar v1, const fpscalar v2) const
	    {
	    	return glm::dot(this->bcweights, fpvec3{v0, v1, v2});
	    }
	    inline int lerpU8I(const int v0, const int v1, const int v2) const
	    {
	    	return ((v0 * this->bcweights_u8i.x) + (v1 * this->bcweights_u8i.y) + (v2 * this->bcweights_u8i.z)) / 255;
	    }
	    inline uint8_t lerpU8(const uint8_t v0, const uint8_t v1, const uint8_t v2) const
	    {
	    	return uint8_t(glm::clamp(this->lerpU8I(v0, v1, v2), 0, 255));
	    }

	    template <glm::length_t L, glm::qualifier Q>
	    inline _fpvecNQ<L,Q> lerpF(const _fpvecNQ<L,Q>& v0, const _fpvecNQ<L,Q>& v1, const _fpvecNQ<L,Q>& v2) const
	    {
	    	return (v0 * this->bcweights.x) + (v1 * this->bcweights.y) + (v2 * this->bcweights.z);
	    }

	    template <glm::length_t L, glm::qualifier Q>
	    inline _ivecNQ<L,Q> lerpU8I(const _ivecNQ<L,Q>& v0, const _ivecNQ<L,Q>& v1, const _ivecNQ<L,Q>& v2) const
	    {
	    	return ((v0 * this->bcweights_u8i.x) + (v1 * this->bcweights_u8i.y) + (v2 * this->bcweights_u8i.z)) / 255;
	    }

	    template <glm::length_t L, glm::qualifier Q>
	    inline _u8vecNQ<L,Q> lerpU8(const _u8vecNQ<L,Q>& v0, const _u8vecNQ<L,Q>& v1, const _u8vecNQ<L,Q>& v2) const
	    {
	        const _ivecNQ<L,Q> iv0(v0);
	        const _ivecNQ<L,Q> iv1(v1);
	        const _ivecNQ<L,Q> iv2(v2);

	    	return _u8vecNQ<L,Q>(glm::clamp(this->lerpU8I(iv0, iv1, iv2), 0, 255));
	    }
	    template <class T>
	    inline T lerpF(const T* vals)
	    {
	    	return this->lerpF(vals[0], vals[1], vals[2]);
	    }
	    template <class T>
	    inline T lerpU8I(const T* vals)
	    {
	    	return this->lerpU8I(vals[0], vals[1], vals[2]);
	    }

	    template <class T>
	    inline T lerpU8(const T* vals)
	    {
	    	return this->lerpU8(vals[0], vals[1], vals[2]);
	    }

	    _tri_barycentric_holder& operator=(const fpvec3& newbcw)
	    {
	    	this->bcweights = newbcw;
	    	this->bcweights_u8i = ivec3(glm::round(newbcw * 255.0));
	    	return *this;
	    }
	    /*
	    inline bool outside_tri() const {
	    	return ((this->bcweights_u8i.x | this->bcweights_u8i.y | this->bcweights_u8i.z) < 0);
	    }
	    */
	    inline bool outside_tri() const {
	    	//return glm::any(glm::lessThan(this->bcweights, fpvec3(0))) || glm::any(glm::greaterThan(this->bcweights, fpvec3(1)));
	    	return glm::any(glm::lessThanEqual(this->bcweights, fpvec3(-DBL_EPSILON)));
	    }
	};
	struct drawpixel_args {
	    fpmat4 mvp, mdl, view, viewinv, mdlview;
	    fpvec3 campos, mdlpos;
	    int width, height;
	    Material& cmat;

	};
	#define DJ_SW3D20_MAX_LIGHTS 128

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

    struct Context {
		//friend struct _ContextManageableObject;
		private:
		    RenderPipelineSettings _rps;
		    Framebuffer *mainfbo = nullptr, *curfbo = nullptr;
		    //bool _owns_mainfbo = false;
		    bool is_locked = false;
		    _ctxobjmap_t slaves;
		    size_t _curidnum = 1;
		    size_t _tri_num_splits = 1;
		    size_t _maxnthreads = 1;

		    size_t _num_lights = 0;
		    struct _lightsrc_entry {
				bool _in_use = false;
				LightSource light;
			};

			_lightsrc_entry _lightsources[DJ_SW3D20_MAX_LIGHTS] = {};

		    _math::tri2d_interpolator* _trilerps[2] = {};
		    Material _default_material = {
				.base_diffuse_color={0,0,0,1},
				.base_specular_color={0,0,0,1},
                .vertex_color_paint={.use=true, .amount=fpvec4(1)},
                .shadeless=true
			};
			Material _q3bigchar_material = {
				.base_diffuse_color={1,1,1,1},
				.shadeless=true,
				.uses_transparency=true,
				.texture_slots=texslotlist_t{
				    MaterialTextureSlot{.src_image=nullptr, .affects_diffuse_color={true, fpvec3(1.0)}, .affects_diffuse_alpha={true, 1.0}, .layerblendtype=enum_t::BLEND_MUL}
				}
			};
		    Material* curmat = nullptr;
		    inline void _sr_writecolor(const ivec2& xy, const u8rgba& color)
		    {
				uint8_t* rbp = this->curfbo->colorptr(xy.x, xy.y);
				if (rbp == nullptr){return;}
				#define _wrch(_i, _v) (this->_rps.cap_draw_color_buffer[_i] ? (rbp[_i] = (_v)) : (_v))
				switch (this->curfbo->rbuf_fmt)
				{
					case enum_t::GRAYSCALE:
					{
						_wrch(0, _math::rgb2gray(u8rgb(color)));
						break;
					}
					case enum_t::GRAYALPHA:
					{
						_wrch(0, _math::rgb2gray(u8rgb(color)));
						_wrch(1, color[3]);
						break;
					}
					case enum_t::RGB:
					{
					    _wrch(0, color[0]);
					    _wrch(1, color[1]);
					    _wrch(2, color[2]);
					    break;
					}
					case enum_t::RGBA:
					{
					    _wrch(0, color[0]);
					    _wrch(1, color[1]);
					    _wrch(2, color[2]);
					    _wrch(3, color[2]);
					    break;
					}
					default: break;
				}
				#undef _wrch
			}
			inline void _sr_writedepth(const ivec2& xy, const fpscalar depth)
			{
				fpscalar* dbp = this->curfbo->depthptr(xy.x, xy.y);
				if (dbp == nullptr || (!this->_rps.cap_draw_depth_buffer)){return;}
				*dbp = depth;
			}
			template <class T>
			T* _new_managed_obj()
			{
				T* ret = new T;
				ret->_cmo_setup(this->slaves, this->_curidnum++);
				return ret;
			}
			inline fpmat4 _calc_mvp()
			{
				return fpmat4(this->_matrices[0])*glm::inverse(fpmat4(this->_matrices[1]))*fpmat4(this->_matrices[2]);
			}
			inline _matstack<fpscalar, 4, 4>& _get_cur_ms()
			{
				return this->_matrices[int(this->_rps.en_matrix_mode)];
			}
			inline fpscalar _get_aspect_ratio()
			{
				return fpscalar(this->curfbo->width)/fpscalar(this->curfbo->height);
			}
			inline fpvec4 _project(const fpmat4& mvp, const fpvec3& pt)
			{
				const fpvec4 ppt = mvp*fpvec4(pt, 1);
			    return fpvec4(fpvec3(ppt)/(ppt.w), ppt.w);
		    }
			/*
			inline uint8_t _get_alpha(const u8rgba& src, const u8rgba& dst)
			{
				switch (this->_rps.en_blend_alpha_src)
				{
					case enum_t::ONE_MINUS_SRC_ALPHA: return uint8_t(glm::clamp(255 - int(src[3]), 0, 255));
					default: return src[3];
				}
			}
			*/
		public:
		    _lightsrc_entry* light_sources = _lightsources;
		    _matstack<fpscalar, 4, 4> _matrices[3] = {};
		    _matstack<fpscalar, 4, 4>& projection_matrix = _matrices[0];
		    _matstack<fpscalar, 4, 4>& view_matrix = _matrices[1];
		    _matstack<fpscalar, 4, 4>& model_matrix = _matrices[2];


		    inline fpscalar srGetAspectRatio() const {return fpscalar(this->curfbo->width)/(this->curfbo->height);}

		    Framebuffer* srCreateManagedFBO(){return this->_new_managed_obj<Framebuffer>();}
		    Texture* srCreateManagedTexture(){return this->_new_managed_obj<Texture>();}

		    template <class T>
		    T* srCreateManaged(){return this->_new_managed_obj<T>();}

		    template <class T>
		    size_t srCreateManaged(const size_t n, T** handles)
		    {
		        size_t i;
		    	for (i = 0; i < n; i++)
		    	{
		    		handles[i] = this->_new_managed_obj<T>();
		    	}
		    	return i;
		    }

		    fpmat4 srCalcMVPMatrix(){return this->_calc_mvp();}
		    bool srIsEnabled(const enum_t cap)
		    {
				bool* ptr = this->_rps._valptr_b(cap);
				return (ptr != nullptr ? *ptr : false);
			}
		    bool srSetEnabled(const enum_t cap, const bool newval)
		    {
				if (this->is_locked){return false;}
				bool* ptr = this->_rps._valptr_b(cap);
				if (ptr == nullptr){return false;}
				else
				{
					bool oldval = *ptr; *ptr = newval;
					return oldval;
				}
			}

			enum_t srCullFace(const enum_t newmode=enum_t::_GET_CURRENT_VALUE)
			{
				enum_t& evr = this->_rps.en_cull_face;
				enum_t oldv = evr;
				if (this->is_locked || newmode == enum_t::_GET_CURRENT_VALUE){return oldv;}
				else if (newmode == enum_t::FRONT_FACE || newmode == enum_t::BACK_FACE){evr = newmode; return oldv;}
				else {throw std::runtime_error("bad enum value!");}
			}

			enum_t srDepthFunc(const enum_t newmode=enum_t::_GET_CURRENT_VALUE)
			{
				enum_t& evr = this->_rps.en_depth_func;
				enum_t oldv = evr;
				if (this->is_locked || newmode == enum_t::_GET_CURRENT_VALUE){return oldv;}
				else if (int(newmode) >= int(enum_t::CMP_LESS) || int(newmode) <= int(enum_t::CMP_NOTEQUAL)){evr = newmode; return oldv;}
				else {throw std::runtime_error("bad enum value!");}
			}

			enum_t srDrawType(const enum_t newmode=enum_t::_GET_CURRENT_VALUE)
			{
				enum_t& evr = this->_rps.en_draw_type;
				enum_t oldv = evr;
				if (this->is_locked || newmode == enum_t::_GET_CURRENT_VALUE){return oldv;}
				else if (int(newmode) >= int(enum_t::DRAW_POINTS) || int(newmode) <= int(enum_t::DRAW_TRIANGLES)){evr = newmode; return oldv;}
				else {throw std::runtime_error("bad enum value!");}
			}
			enum_t srMatrixMode(const enum_t newmode=enum_t::_GET_CURRENT_VALUE)
			{
				enum_t& evr = this->_rps.en_matrix_mode;
				enum_t oldv = evr;
				if (this->is_locked || newmode == enum_t::_GET_CURRENT_VALUE){return oldv;}
				else if (int(newmode) >= 0 || int(newmode) <= 2){evr = newmode; return oldv;}
				else {throw std::runtime_error("bad enum value!");}
			}

			fpvec4 srClearColor() const {
				return this->_rps.clear_color;
			}
			fpscalar srClearDepth() const {
				return this->_rps.clear_depth;
			}
			fpvec4 srClearColor(const fpscalar r, const fpscalar g, const fpscalar b, const fpscalar a=1.0)
			{
				if (this->is_locked){return this->_rps.clear_color;}
				fpvec4 occ = this->_rps.clear_color;
				this->_rps.clear_color = fpvec4{r,g,b,a};
				return occ;
			}
			fpscalar srClearDepth(const fpscalar depth=1.0)
			{
				if (this->is_locked){return this->_rps.clear_depth;}
				fpscalar occ = this->_rps.clear_depth;
				this->_rps.clear_depth = depth;
				return occ;
			}
			Framebuffer* srActiveFBO()
			{
				return this->curfbo;
			}
			Framebuffer* srActiveFBO(Framebuffer* newfbo)
			{
				if (this->is_locked){return nullptr;}
				Framebuffer* oldfbo = this->curfbo;
				this->curfbo = (newfbo != nullptr ? newfbo : this->mainfbo);
				return oldfbo;
			}

			Material* srActiveMaterial()
			{
				return this->curmat;
			}
			Material* srActiveMaterial(Material* newmat)
			{
				if (this->is_locked){return this->curmat;}
				auto oldmat = this->curmat;
				this->curmat = newmat;
				return oldmat;
			}

			LightSource* srCreateLightSource() {
				if (this->_num_lights >= DJ_SW3D20_MAX_LIGHTS){return nullptr;}
				for (size_t i = 0; i < DJ_SW3D20_MAX_LIGHTS; i++) {
					auto& _ld = this->_lightsources[i];
					if (!_ld._in_use){
						_ld._in_use = true;
						_ld.light = LightSource{};
						this->_num_lights++;
						return &_ld.light;
					}
				}
				return nullptr;
			}

			void srResetLightSources() {
                std::fill_n(this->_lightsources, DJ_SW3D20_MAX_LIGHTS, _lightsrc_entry{});
                this->_num_lights = 0;
            }

			void srPushMatrix()
			{
				this->_get_cur_ms().srPushMatrix();
			}
			bool srPopMatrix()
			{
				return this->_get_cur_ms().srPopMatrix();
			}

			void srMultMatrix(const fpmat4& m)
			{
				this->_get_cur_ms().srMultMatrix(m);
			}
			void srTranslatef(const fpscalar x, const fpscalar y, const fpscalar z=0)
			{
			    fpmat4& top = this->srTopMatrix();
				top = glm::translate(top, fpvec3{x,y,z});
			}
			void srRotatef(const fpscalar angle_degrees, const fpscalar ax=0, const fpscalar ay=0, const fpscalar az=1)
			{
			    fpmat4& top = this->srTopMatrix();
				top = (glm::rotate(top, glm::radians(angle_degrees), glm::normalize(fpvec3{ax,ay,az})));
			}
			void srScalef(const fpscalar sx, const fpscalar sy, const fpscalar sz=1)
			{
				this->srTopMatrix() *= (glm::scale(fpvec3{sx, sy, sz}));
			}

			void srLoadIdentity()
			{
				this->_get_cur_ms().srLoadIdentity();
			}
			void srReloadParentMatrix()
			{
				this->_get_cur_ms().srReloadParent();
			}
			fpmat4& srTopMatrix()
			{
				return this->_get_cur_ms().srTopMatrix();
			}
			void srFrustum(const fpscalar left, const fpscalar right, const fpscalar bottom, const fpscalar top, const fpscalar znear=0.1, const fpscalar zfar=100.0)
			{
				this->srTopMatrix() = glm::frustum(left, right, bottom, top, znear, zfar);
			}
			void srPerspective(const fpscalar fov, const fpscalar z_near=0.1, const fpscalar z_far=100.0)
			{
			    const fpscalar _ffov = glm::radians(fov);
			    fpmat4& top = this->srTopMatrix();
				top = glm::perspectiveFovRH_NO(_ffov, fpscalar(this->curfbo->width), fpscalar(this->curfbo->height), z_near, z_far);
			}
			void srPerspectiveX_FOV(const fpscalar fov_x, const fpscalar z_near=0.1, const fpscalar z_far=100.0)
			{
				const fpscalar aspect = fpscalar(this->curfbo->width)/this->curfbo->height;
				this->srPerspective(glm::clamp(fov_x / aspect, 0.001, 179.999), z_near, z_far);
			}
			void srClearBuffers(const bool colorbuffer=true, const bool depthbuffer=true)
			{
				if (this->is_locked){return;}

				const size_t npx = this->curfbo->width * this->curfbo->height;

				if (colorbuffer)
				{
					const u8rgba clcolu8 = _math::fp2u8(this->_rps.clear_color);

					switch (this->curfbo->rbuf_fmt)
					{
						case enum_t::GRAYSCALE:
						    memset(this->curfbo->renderbuffer, clcolu8[0], npx);
						    break;
						case enum_t::GRAYALPHA:
						    std::fill_n((u8ra*)(this->curfbo->renderbuffer), npx, u8ra{clcolu8[0], clcolu8[3]});
					        break;
					    case enum_t::RGB:
					        std::fill_n((u8rgb*)(this->curfbo->renderbuffer), npx, u8rgb(clcolu8));
					        break;
					    case enum_t::RGBA:
					        std::fill_n((u8rgba*)(this->curfbo->renderbuffer), npx, clcolu8);
					        break;
					    default: break;
					}
				}
				if (depthbuffer)
				{
					std::fill_n(this->curfbo->depthbuffer, npx, this->_rps.clear_depth);
				}
			}

			void srContextShutdown()
			{

				for (int i = 0; i < 3; i++)
				{
				    auto& ms = this->_matrices[i];
					while (ms.srPopMatrix()){;}
					ms.srLoadIdentity();
				}

				while (this->slaves.size() > 0)
				{
					_ContextManageableObject* cmo = (*(this->slaves.begin())).second;
					delete cmo;
				}
				this->_curidnum = 1;
				this->mainfbo = nullptr;
				this->curfbo = nullptr;
				this->is_locked = false;
				//this->_trilerps[0] = _math::tri2d_interpolator();
				//this->_trilerps[1] = _math::tri2d_interpolator();

				//this->_matrices[0] = _matstack<fpscalar, 4, 4>(64);
				//this->_matrices[1] = _matstack<fpscalar, 4, 4>(64);
				//this->_matrices[2] = _matstack<fpscalar, 4, 4>(64);
			}
			void srContextInit(Framebuffer& fbo)
			{
			    this->srContextShutdown();
				this->is_locked = false;
				this->_rps = RenderPipelineSettings();
				this->curfbo = this->mainfbo = &fbo;
			}
			void srContextInit(const int w, const int h, const enum_t rbf=enum_t::RGB, uint8_t* rbuf=nullptr, fpscalar* dbuf=nullptr)
			{
			    this->srContextShutdown();
				this->is_locked = false;
				this->_rps = RenderPipelineSettings();
				this->curfbo = this->mainfbo = this->srCreateManagedFBO();
				this->mainfbo->init(w, h, rbf, rbuf, dbuf);
			}
			/*
			void srDrawElements(const size_t nverts, const ffvertex_t* vertices)
			{
				if (this->is_locked){throw std::runtime_error("Can't draw while context is locked!");}
			    this->is_locked = true;

			    const fpmat4 mvp = this->_calc_mvp();
   				const fpmat4 mdl(this->model_matrix);
   				const fpmat4 view(this->view_matrix);
   				const fpmat4 viewinv = glm::inverse(view);
   				const fpmat4 mdlview = viewinv*mdl;
   				const fpvec3 campos(view[3]);
   				const fpvec3 mdlpos(mdl[3]);

   				const int width = this->curfbo->width;
   				const int height = this->curfbo->height;

   				const Material& cmat = (this->curmat != nullptr ? *(this->curmat) : (this->_default_material));
			    this->is_locked = false;
			}
			*/
			struct _drawelems_header1 {
				fpmat4 mvp, mdl, view, viewinv, mdlview;
				fpvec3 campos, mdlpos;
                fpscalar znear, zfar;
				int width, height;
				const Material* cmatptr;
				bool
				    must_calc_mirrorball,
				    must_calc_greflvec,
				    must_calc_gviewvec,
				    must_calc_fragpos_world,
				    must_calc_fragpos_view,
				    must_calc_worldnormal,
				    must_calc_viewnormal,
				    must_calc_viewvec_w,
				    must_calc_viewvec
				;
				size_t ntexslots;
				const MaterialTextureSlot* texslots;
				u8rgba
				    base_diffuse_color,
				    base_specular_color,
				    vcl_u8,
				    vcp_u8
				;
				aabb2Di screenbounds;
			};
			void _sr_plotrawtri(const _drawelems_header1& hdr, const size_t trinum, const fpvec4* oprjco, const fpvec3* base_affines) {
			}
			void srDrawElements(const size_t nverts, const ffvertex_t* vertices)
			{
				if (this->is_locked){throw std::runtime_error("Can't draw while context is locked!");}
				this->is_locked = true;
                const fpmat4 prjinv = glm::inverse(fpmat4(this->projection_matrix));
				const fpmat4 mvp = this->_calc_mvp();
				const fpmat4 mdl(this->model_matrix);
				const fpmat4 view(this->view_matrix);
				const fpmat4 viewinv = glm::inverse(view);
				const fpmat4 mdlview = viewinv*mdl;
				const fpvec3 campos(view[3]);
				const fpvec3 mdlpos(mdl[3]);
				//const size_t primsize = _drawtype_primitive_sizes[int(this->_rps.en_draw_type)];
				const int width = this->curfbo->width;
				const int height = this->curfbo->height;

				const Material& cmat = (this->curmat != nullptr ? *(this->curmat) : (this->_default_material));

				fpmat2 bcmatrix;

				//fpvec3 affine_corners[4], perspective_corners[4];
				//fpvec3 pc_wvals, pc_invwvals;

				bool must_calc_mirrorball = false;
				bool must_calc_greflvec = false;
				bool must_calc_gviewvec = false;
				for (auto& cts : cmat.texture_slots)
				{
					if (cts.mapping_type == enum_t::TEXMAP_REFLECTION)
					{
						if (cts.textype == enum_t::TEXTYPE_IMAGE)
						{
							must_calc_mirrorball = true;
						}
						else if (cts.textype == enum_t::TEXTYPE_ENVMAP)
						{
							must_calc_greflvec = true;
						}
					}
					else if (cts.mapping_type == enum_t::TEXMAP_VIEWVECTOR)
					{
						must_calc_gviewvec = true;
					}
				}

				const bool must_calc_fragpos_world = must_calc_greflvec || must_calc_gviewvec;
				const bool must_calc_fragpos_view = must_calc_mirrorball;
				const bool must_calc_worldnormal = must_calc_greflvec;
				const bool must_calc_viewnormal = must_calc_mirrorball;

				const bool must_calc_viewvec_w = must_calc_greflvec || must_calc_gviewvec;
				const bool must_calc_viewvec = must_calc_mirrorball;

				const size_t ntexslots = cmat.texture_slots.size();
				const MaterialTextureSlot* texslots = cmat.texture_slots.data();

				const u8rgba base_diffuse_color = _math::fp2u8(cmat.base_diffuse_color);
				const u8rgba base_specular_color = _math::fp2u8(cmat.base_specular_color);
				const u8rgba vcl_u8(_math::fp2u8(cmat.vertex_color_light.amount));
				const u8rgba vcp_u8(_math::fp2u8(cmat.vertex_color_paint.amount));

				const aabb2Di screenbounds = {
					ivec2{0,0},
					ivec2{width-1, height-1}
				};

				_drawelems_header1 _header = {};
                const fpvec4 near_unproj = prjinv * fpvec4{0,0,-1,1};
                const fpvec4 far_unproj = prjinv * fpvec4{0,0,1,1};
                _header.znear = (near_unproj.z / near_unproj.w);
                _header.zfar = (far_unproj.z / far_unproj.w);
                if (_header.zfar > _header.znear){std::swap(_header.znear, _header.zfar);}
				#define _hdrtf(_n) _header._n = _n

				_hdrtf(mvp);
				_hdrtf(mdl);
				_hdrtf(view);
				_hdrtf(viewinv);
				_hdrtf(mdlview);

				_hdrtf(mdlpos);
				_hdrtf(campos);

				_hdrtf(must_calc_fragpos_view);
				_hdrtf(must_calc_fragpos_world);
				_hdrtf(must_calc_greflvec);
				_hdrtf(must_calc_gviewvec);
				_hdrtf(must_calc_mirrorball);
				_hdrtf(must_calc_viewnormal);
				_hdrtf(must_calc_viewvec);
				_hdrtf(must_calc_viewvec_w);
				_hdrtf(must_calc_worldnormal);

				_hdrtf(screenbounds);
				_hdrtf(ntexslots);
				_hdrtf(texslots);
				_header.cmatptr = &cmat;
				_hdrtf(base_diffuse_color);
				_hdrtf(base_specular_color);
				_hdrtf(vcl_u8);
				_hdrtf(vcp_u8);

				_hdrtf(width);
				_hdrtf(height);

				#undef _hdrtf
				switch (this->_rps.en_draw_type)
				{
					case enum_t::DRAW_TRIANGLES:
					{
						this->_sr_start_tri_draw(_header, nverts, vertices);
						break;
					}
					default: break;
				}
				this->is_locked = false;
			}

			struct _drawtri_header1 {
				_drawelems_header1* _hdrptr;
				const ffvertex_t* overts[3];
                ffvertex_t vverts[3];
				const ffvertex_t* finalverts;
				fpvec3
				    worldco[3],
				    viewco[3],
				    worldnormals[3],
				    viewnormals[3]
				;
				fpscalar
				    zvalues[3]
				;
				u8rgba
				    vcolors_u8[3]
				;
				fpvec4 prjco[3];
				size_t splitnum;

				aabb2Di tribox;

				fpvec3
				    affine_corners[4],
				    pc_wvals, pc_invwvals,
				    pc_corners[4]
				;
				_tri_barycentric_holder trilerp2, trilerp2pc;
				int x, y;
				fpscalar
				    xfincr, yfincr,
				    xfactor, yfactor
				;
				fpvec3 affine_bcw, pc_bcw;

				fpvec3
				    fragpos_world,
				    fragpos_view,
				    viewvec_w,
				    viewvec,
				    worldnorm,
				    viewnorm,
				    mirrorball_refl,
				    greflvec,
                    //tangent_vector,
                    //binormal_vector,
                    up_vector,

                    //vtangents[3],
                    //vbinormals[3],
                    //vupvecs[3]
                    face_tangents[16],
                    face_binormals[16]
				;

				fpvec2
				    mirrorball_uv
				;
				fpscalar frag_lod, frag_z;
				fpvec4
				    frag_base_diffuse
				;
			};

			void _sr_drawtri_plotfrag_basic_setup1(_drawtri_header1& _dthdr) {
				_drawelems_header1& _hdr = *_dthdr._hdrptr;
				const Material& cmat = *_hdr.cmatptr;
				_tri_barycentric_holder &trilerp2 = _dthdr.trilerp2, &trilerp2pc = _dthdr.trilerp2pc;

				#define _dthdr_fieldref(_n) auto& _n = _dthdr._n

			    _dthdr_fieldref(fragpos_world);
			    _dthdr_fieldref(fragpos_view);
			    _dthdr_fieldref(viewvec_w);
			    _dthdr_fieldref(viewvec);
			    _dthdr_fieldref(worldnorm);
			    _dthdr_fieldref(viewnorm);
			    _dthdr_fieldref(mirrorball_refl);
			    _dthdr_fieldref(mirrorball_uv);
			    _dthdr_fieldref(greflvec);
			    _dthdr_fieldref(frag_lod);
			    _dthdr_fieldref(frag_base_diffuse);
			    //_dthdr_fieldref(frag_base_specular);
			    #undef _dthdr_fieldref

				auto* worldco = _dthdr.worldco;
				auto* viewco = _dthdr.viewco;

				_dthdr.fragpos_world = (_hdr.must_calc_fragpos_world || true ? trilerp2pc.lerpF(worldco) : _hdr.campos+fpvec3{0,1,0});
    			_dthdr.fragpos_view = (_hdr.must_calc_fragpos_view || true ? trilerp2pc.lerpF(viewco) : fpvec3{0,0,-1});
				_dthdr.viewvec_w = (_hdr.must_calc_viewvec_w || true ? glm::normalize(fragpos_world-_hdr.campos) : fpvec3{0,1,0});
				_dthdr.viewvec = (_hdr.must_calc_viewvec || true ? glm::normalize(fragpos_view) : fpvec3{0,0,-1});
				_dthdr.worldnorm = (_hdr.must_calc_worldnormal || true ? glm::normalize(trilerp2pc.lerpF(_dthdr.worldnormals)) : fpvec3{0,0,1});
				_dthdr.viewnorm = (_hdr.must_calc_viewnormal || true ? glm::normalize(trilerp2pc.lerpF(_dthdr.viewnormals)) : fpvec3{0,0,1});



				_dthdr.mirrorball_refl = (_hdr.must_calc_mirrorball ? glm::reflect(viewvec, viewnorm) : fpvec3{0,0,0});

				_dthdr.mirrorball_uv = {0.5 + (0.4 * mirrorball_refl.x), 0.5 + (0.4 * mirrorball_refl.y)};

				greflvec = (_hdr.must_calc_greflvec || true ? glm::reflect(viewvec_w, worldnorm) : fpvec3{0,0,1});

				const fpvec4 vcolor = fpvec4(trilerp2pc.lerpU8(_dthdr.vcolors_u8)) / fpvec4(255.0);

				_dthdr.frag_z = trilerp2.lerpF(_dthdr.zvalues);
				const fpscalar lod = 0.5 + (0.5 * _dthdr.frag_z);
				//const fpscalar lod = 0.0;
				frag_lod = lod;

				fpvec4 base_diffuse = cmat.base_diffuse_color;
				if (cmat.vertex_color_paint.use){
					base_diffuse = glm::mix(base_diffuse, vcolor, cmat.vertex_color_paint.amount);
				}
				if (cmat.vertex_color_light.use){
					base_diffuse = _math::blend(cmat.vertex_color_light_blend, base_diffuse, vcolor, cmat.vertex_color_light.amount);
				}
				_dthdr.frag_base_diffuse = base_diffuse;
			}
			fpvec4 _sr_drawtri_plotfrag_process_lighting(
			    _drawtri_header1& _dthdr,
			    const fpscalar dintensity,
			    const fpscalar sintensity,
			    const fpscalar shardness,
			    const fpvec4& bdcolor,
			    const fpvec4& dcolor,
			    const fpvec4& bscolor,
			    const fpvec4& scolor
			){

				auto& _hdr = *_dthdr._hdrptr;
				const Material& cmat = *_hdr.cmatptr;
			    if (cmat.shadeless){return dcolor;}
				fpvec4 bdiffuse = bdcolor, mdiffuse = dcolor, cdiffuse = {0,0,0,0}, bspecular = bscolor, mspecular = scolor, cspecular = {0,0,0,0};
				fpvec3 fragposw = _dthdr.fragpos_world;
				fpvec3 fragnormw = glm::normalize(_dthdr.worldnorm);
				fpvec3 viewvec_w = glm::normalize(fragposw - _hdr.campos);
				fpvec3 specreflvec = glm::reflect(viewvec_w, fragnormw);
				for (size_t i = 0; i < DJ_SW3D20_MAX_LIGHTS; i++) {
					auto& lsrce = this->_lightsources[i];
					if (!lsrce._in_use || !lsrce.light.active){continue;}
					auto& lsrc = lsrce.light;
					fpscalar lfac = 0, sfac = 0;
					fpvec3 lightdelta = {0,0,0}, lightvec = {0,0,0};
					bool do_spec = true;
					switch (lsrc.type) {
						case light_type::point: {
							lightdelta = (lsrc.pos - fragposw);
						    lightvec = glm::normalize(lightdelta);
							lfac = glm::max(glm::dot(lightvec, fragnormw), 0.0) * _calc_light_factor(lsrc.falloff, glm::length(lightdelta), lsrc.point_radius_min, lsrc.point_radius_max);
							break;
						}
						case light_type::spot: {
							const fpvec3 lightfwd = glm::normalize(lsrc.orientation * fpvec3{0,0,-1});
							lightdelta = (lsrc.pos - fragposw);
							lightvec = glm::normalize(lightdelta);
							const fpscalar spotdot = glm::dot(-lightvec, lightfwd);
							const fpscalar lvecangle = glm::degrees(glm::acos(spotdot));
							if (spotdot <= 0.0 || lvecangle > (lsrc.spot_cone_angle*0.5)){lfac = 0.0; continue;}
							const fpscalar spotdist = glm::dot(lightfwd, fragposw - lsrc.pos);
							//if (glm::dot(lightfwd, -fragnormw) < 0.0){continue;}
							if (spotdist < 0 || spotdist >= lsrc.spot_max_distance){continue;}
							lfac = glm::max(glm::dot(lightvec, fragnormw) * (1.0 - (lvecangle / (lsrc.spot_cone_angle*0.5))), 0.0) * _calc_light_factor(lsrc.falloff, spotdist, 0.0, lsrc.spot_max_distance);
							break;
						}
						case light_type::hemi: {
						    const fpvec3 lightfwd = glm::normalize(lsrc.orientation * fpvec3{0,0,-1});
						    lightvec = -lightfwd;
						    const fpscalar ldot = glm::dot(lightvec, fragnormw);
                            lfac = glm::max(glm::mix(fpscalar(0.5), fpscalar(1), ldot), fpscalar(0));
                            break;
						}
						case light_type::ambient: {
							do_spec = false;
							lfac = dintensity;
							break;
						}
						default: continue;
					}
					if (do_spec && lsrc.specular_intensity > 0 && sintensity > 0) {
						sfac = glm::pow(glm::dot(specreflvec, lightvec), shardness);
					}
				    else {
						sfac = 0;
					}
					cdiffuse += (lsrc.diffuse_color * lsrc.diffuse_intensity * 0.1 * lfac);
					cspecular += (lsrc.specular_color * lsrc.specular_intensity * 0.1 * lfac * sfac);
				}
				fpvec4 finalcolor = glm::mix(bdiffuse, mdiffuse, cdiffuse*dintensity);
				finalcolor += glm::mix(bspecular, mspecular, glm::max(fpvec4(0.0), cspecular*sintensity));
				return finalcolor;
			}
			struct _sr_texturing_result {
				//u8rgb diffuse_color, specular_color;
				//uint8_t diffuse_alpha, specular_alpha;
				fpvec3 diffuse_color, specular_color;
				fpscalar diffuse_alpha, specular_alpha;
				fpscalar diffuse_intensity, specular_intensity, specular_hardness;
                fpvec3 cur_texcoord_offset = {0,0,0};
			};
			void _sr_applytexture(_drawtri_header1& _dthdr, _sr_texturing_result& res, const MaterialTextureSlot& mts) {
				if (!mts.use){return;}
				auto& trilerp2pc = _dthdr.trilerp2pc;
				auto& trilerp2 = _dthdr.trilerp2;
				const auto& v0 = _dthdr.finalverts[0];
				const auto& v1 = _dthdr.finalverts[1];
				const auto& v2 = _dthdr.finalverts[2];
				u8rgba texel = {0,0,0,0};
				fpvec4 texelf(0);
                fpvec3 tc_offset = res.cur_texcoord_offset + mts.texcoord_offset;
                const fpmat4 model_matrix = _dthdr._hdrptr->mdl;
                const fpmat4 view_matrix = _dthdr._hdrptr->view;
                const fpmat4 view_matrix_inv = _dthdr._hdrptr->viewinv;
                const fpvec3 ogwnorm = glm::normalize(_dthdr.worldnorm);
				switch (mts.textype) {
					case enum_t::TEXTYPE_IMAGE: {
						if (mts.src_image == nullptr){break;}
						switch (mts.mapping_type) {
							case enum_t::TEXMAP_UVMAP: {
								if (mts.uvmapno >= 0 && mts.uvmapno < 16)
								{
									const fpvec2 uv = trilerp2pc.lerpF(v0.uv[mts.uvmapno], v1.uv[mts.uvmapno], v2.uv[mts.uvmapno]);
									texel = mts.src_image->sample2D(uv + fpvec2(tc_offset), _dthdr.frag_lod);
								}
						        break;
							}
							case enum_t::TEXMAP_REFLECTION: {
								texel = mts.src_image->sample2D(_dthdr.mirrorball_uv + fpvec2(tc_offset), _dthdr.frag_lod);
								break;
							}
							default: ;
						}
						break;
					}
                    case enum_t::TEXTYPE_ENVMAP: {
                        if (mts.src_envmap == nullptr){break;}
                        switch (mts.envmaptype) {
                            case enum_t::ENVMAP_EQUIRECT: {
                                switch (mts.mapping_type){
                                    case enum_t::TEXMAP_REFLECTION: {
                                        texel = mts.src_envmap->sampleEquirect(glm::normalize(glm::reflect(_dthdr.viewvec_w, _dthdr.worldnorm) + tc_offset), 0);
                                        break;
                                    }
                                    case enum_t::TEXMAP_VIEWVECTOR: {
                                        texel = mts.src_envmap->sampleEquirect(glm::normalize(_dthdr.viewvec_w + tc_offset), 0);
                                        break;
                                    }
                                    default: ;
                                }
                                break;
                            }
                            case enum_t::ENVMAP_CUBEMAP: {
                                switch (mts.mapping_type){
                                    case enum_t::TEXMAP_REFLECTION:{
                                        texel = mts.src_envmap->sampleCube(glm::normalize(glm::reflect(_dthdr.viewvec_w, _dthdr.worldnorm) + tc_offset), 0);
                                        break;
                                    }
                                    case enum_t::TEXMAP_VIEWVECTOR:{
                                        texel = mts.src_envmap->sampleCube(glm::normalize(_dthdr.viewvec_w + tc_offset), 0);
                                        break;
                                    }
                                    default: ;
                                }
                                break;
                            }
                            default: ;
                        }
                    }
					default: ;
				}
				texelf = fpvec4(texel)/fpvec4(255);
				if (mts.affects_diffuse_color.use) {
					//res.diffuse_color = _math::u8blend(mts.layerblendtype, res.diffuse_color, u8rgb(texel), _math::fp2u8(mts.affects_diffuse_color.amount));
					res.diffuse_color = _math::blend(mts.layerblendtype, res.diffuse_color, fpvec3(texelf), mts.affects_diffuse_color.amount);
				}
				if (mts.affects_specular_color.use) {
					//res.specular_color = _math::u8blend(mts.layerblendtype, res.specular_color, u8rgb(texel), _math::fp2u8(mts.affects_specular_color.amount));
					res.specular_color = _math::blend(mts.layerblendtype, res.specular_color, fpvec3(texelf), mts.affects_specular_color.amount);
				}
				if (mts.affects_diffuse_alpha.use) {
					//res.diffuse_alpha = _math::u8blend(mts.layerblendtype, u8rgb(res.diffuse_alpha), u8rgb(texel), _math::fp2u8(mts.affects_diffuse_color.amount))[0];
				    res.diffuse_alpha = _math::blend(mts.layerblendtype, fpvec2(res.diffuse_alpha), fpvec2(texelf[mts.alpha_channel]), fpvec2(mts.affects_diffuse_alpha.amount))[0];
				}
                if (mts.affects_texcoord_offset.use) {
                    const fpvec3 ofsvec = glm::mix(fpvec3(-1), fpvec3(1), _math::u8tofp(u8rgb(texel)));
                    res.cur_texcoord_offset = _math::blend(mts.layerblendtype, res.cur_texcoord_offset, ofsvec, mts.affects_texcoord_offset.amount);
                }
                if (mts.affects_normal_vector.use) {
                    fpvec3 tgnormal = _dthdr.worldnorm;
                    if (mts.bumpspace == enum_t::BUMPMAPSPACE_OBJECT){
                        tgnormal = glm::normalize(fpvec3(model_matrix * fpvec4(glm::mix(fpvec3(-1), fpvec3(1), fpvec3(texelf)), 0)));
                    }
                    else if (mts.bumpspace == enum_t::BUMPMAPSPACE_TANGENT) {
                        /*
                        fpvec3 snv = glm::mix(fpvec3(-1), fpvec3(1), fpvec3(texelf));
                        fpvec3 fnv = (_dthdr.tangent_vector * snv.x) + (_dthdr.binormal_vector * snv.y) + (_dthdr.up_vector * snv.z);
                        tgnormal = glm::normalize(fnv);
                        */
                        fpmat3 tbn = fpmat3{_dthdr.face_tangents[mts.uvmapno], -_dthdr.face_binormals[mts.uvmapno], ogwnorm};
                        tgnormal = glm::normalize(tbn * glm::mix(fpvec3(-1), fpvec3(1), fpvec3(texelf)));
                    }
                    _dthdr.worldnorm = glm::normalize(_math::blend(mts.layerblendtype, _dthdr.worldnorm, tgnormal, mts.affects_normal_vector.amount));
                    _dthdr.viewnorm = glm::normalize(fpvec3(view_matrix_inv * fpvec4(_dthdr.worldnorm, 0)));
                }
			}
			bool _sr_edgeclip(ffvertex_t& out_b, const ffvertex_t& in_a, const ffvertex_t& in_b, const fpscalar& _znear) {
                fpscalar znear = _znear;
				bool apos = in_a.co.z >= znear;
				bool bpos = in_b.co.z >= znear;
				if (apos == bpos){return false;}
				//fpscalar azd = (in_a.co.z - znear), zdsum = azd+(in_b.co.z - znear);
				//fpscalar zdpc = (azd/zdsum);
                fpscalar dsum = glm::length(in_b.co - in_a.co);
                fpvec3 abdir = glm::normalize(in_b.co - in_a.co);
                fpscalar dist = 0;
                glm::intersectRayPlane(in_a.co, abdir, fpvec3{0,0,znear}, fpvec3{0,0,-1}, dist);
                fpscalar zdpc = dist/dsum;
				#define _lerpfield(_fn) out_b._fn = glm::mix(in_a._fn, in_b._fn, zdpc)
				out_b = ffvertex_t{};
				_lerpfield(co);
				_lerpfield(color);
				_lerpfield(normal); if (glm::length(out_b.normal) != 0){out_b.normal = glm::normalize(out_b.normal);}
				for (size_t i = 0; i < 16; i++){_lerpfield(uv[i]);}
				//_lerpfield(tangent);
				//_lerpfield(binormal);
				#undef _lerpfield
				return true;
			}
			void _sr_start_tri_draw(_drawelems_header1& _hdr, const size_t nverts, const ffvertex_t* vertices) {
				aabb2Di triboxes[2];
				_math::tri2d_interpolator _basetrilerps[2];
				const auto width = _hdr.width;
				const auto height = _hdr.height;
				const Material& cmat = *_hdr.cmatptr;

				const fpvec3 frontvec = {0,0,1};

				for (size_t i = 0, j; i < nverts; i += 3)
				{
					const ffvertex_t &v0 = vertices[i+0], &v1 = vertices[i+1], &v2 = vertices[i+2];
					_drawtri_header1 _dthdr = {};

					_dthdr._hdrptr = &_hdr;
					_dthdr.splitnum = 0;

					_dthdr.overts[0] = &v0;
					_dthdr.overts[1] = &v1;
					_dthdr.overts[2] = &v2;

					ffvertex_t viewverts[3] = {v0, v1, v2};
					ffvertex_t splitverts[2][3] = {};
					size_t nsplits = 1;

					viewverts[0].co = fpvec3(_hdr.mdlview * fpvec4(v0.co, 1.0));
					viewverts[1].co = fpvec3(_hdr.mdlview * fpvec4(v1.co, 1.0));
					viewverts[2].co = fpvec3(_hdr.mdlview * fpvec4(v2.co, 1.0));

                    for (size_t uvn = 0; uvn < 16; uvn++){
                        const fpvec3
                            dpos1 = v1.co - v0.co,
                            dpos2 = v2.co - v0.co
                        ;
                        const fpvec2
                            duv1 = v1.uv[uvn] - v0.uv[uvn],
                            duv2 = v2.uv[uvn] - v0.uv[uvn]
                        ;

                        const fpscalar r = 1 / (duv1.x * duv2.y - duv1.y * duv2.x);
                        _dthdr.face_tangents[uvn] = glm::normalize(fpvec3(_hdr.mdl * fpvec4((dpos1 * duv2.y  - dpos2 * duv1.y)*r, 0)));
                        _dthdr.face_binormals[uvn] = glm::normalize(fpvec3(_hdr.mdl * fpvec4((dpos2 * duv1.x - dpos1 * duv2.x)*r, 0)));
                    }

                    std::copy_n(viewverts, 3, _dthdr.vverts);
                    /*
                    fpvec3 ftangent = (v0.tangent + v1.tangent + v2.tangent) / fpvec3(3);
                    fpvec3 fbinormal = (v0.binormal + v1.binormal + v2.binormal) / fpvec3(3);

                    _dthdr.up_vector = glm::normalize(fpvec3(_hdr.mdl * fpvec4(glm::cross(v1.co - v0.co, v2.co - v0.co), 0)));
                    _dthdr.tangent_vector = glm::normalize(fpvec3(_hdr.mdl * fpvec4(ftangent, 0)));
                    _dthdr.binormal_vector = glm::normalize(fpvec3(_hdr.mdl * fpvec4(fbinormal, 0)));
                    */





					/*
					size_t nvertsbehind =
					    size_t(viewverts[0].co.z > 0) +
					    size_t(viewverts[1].co.z > 0) +
					    size_t(viewverts[2].co.z > 0)
					;
					*/

					size_t nvertsbehind = 0, nvertsahead = 0;
					size_t aheadverts[3] = {}, behindverts[3] = {};

					for (size_t _svno = 0; _svno < 3; _svno++) {
						ffvertex_t& vv = viewverts[_svno];
						if (vv.co.z > _hdr.znear) {
							behindverts[nvertsbehind] = _svno;
							nvertsbehind++;
						}
						else {
							aheadverts[nvertsahead] = _svno;
							nvertsahead++;
						}
					}

					if (nvertsbehind == 0) {
						std::copy_n(viewverts, 3, splitverts[0]);
					}

                    else if (nvertsbehind == 1) {
                        size_t backidx = behindverts[0];
                        const size_t* frontidxs = aheadverts;

                        //splitverts[0][frontidxs[0]] = splitverts[1][frontidxs[1]] = viewverts[frontidxs[0]];
                        //splitverts[0][frontidxs[1]] = splitverts[1][frontidxs[0]] = viewverts[frontidxs[1]];

                        ffvertex_t aprime, bprime;
                        this->_sr_edgeclip(aprime, viewverts[frontidxs[0]], viewverts[backidx], _hdr.znear);
                        this->_sr_edgeclip(bprime, viewverts[frontidxs[1]], viewverts[backidx], _hdr.znear);

                        splitverts[0][0] = viewverts[frontidxs[1]];
                        splitverts[0][1] = viewverts[frontidxs[0]];
                        splitverts[0][2] = aprime;

                        splitverts[1][0] = aprime;
                        splitverts[1][1] = bprime;
                        splitverts[1][2] = viewverts[frontidxs[1]];

                        for (int i = 0; i < 2; i++){
                            //splitverts[i][backidx].co.z = _hdr.znear;
                        }



                        nsplits = 2;
                    }

					else if (nvertsbehind == 2) {
						//size_t frontidx = 0;
					    //for (; frontidx < 3 && viewverts[frontidx].co.z <= _hdr.znear; frontidx++){;}
					    size_t frontidx = aheadverts[0];
					    const size_t* _backidxs = behindverts;
					    splitverts[0][frontidx] = viewverts[frontidx];
					    this->_sr_edgeclip(splitverts[0][_backidxs[0]], viewverts[frontidx], viewverts[_backidxs[0]], _hdr.znear);
					    this->_sr_edgeclip(splitverts[0][_backidxs[1]], viewverts[frontidx], viewverts[_backidxs[1]], _hdr.znear);
					    for (size_t _bvno = 0; _bvno < 2; _bvno++) {
							size_t backidx = _backidxs[_bvno];
							//splitverts[0][backidx] = viewverts[backidx];
							splitverts[0][backidx].co.z = _hdr.znear;
						}
					}

					else {
						continue;
						/*
						for (size_t _avno = 0; _avno < nvertsahead; _avno++)
						{
							splitverts[0][aheadverts[_avno]] = viewverts[aheadverts[_avno]];
						}
						for (size_t _bvno = 0; _bvno < nvertsbehind; _bvno++) {
							ffvertex_t& backvert = viewverts[behindverts[_bvno]];
							splitverts[0][behindverts[_bvno]] = backvert;
							splitverts[0][behindverts[_bvno]].co.z = -0.01;
						}
						*/
					}

					for (size_t spliti = 0; spliti < nsplits; spliti++) {
						_dthdr.splitnum = spliti;
						_dthdr.finalverts = splitverts[spliti];
						this->_draw_split_tri(_dthdr);
					}

				}
			}
			void _draw_split_tri(_drawtri_header1& _dthdr) {
				const fpmat4 proj = this->projection_matrix;
				const _drawelems_header1& _hdr = *_dthdr._hdrptr;
				const auto width = _hdr.width;
				const auto height = _hdr.height;
				static const fpvec3 frontvec = {0, 0, 1};
				auto& screenbounds = _hdr.screenbounds;
				fpvec4* oprjco = _dthdr.prjco;
				const ffvertex_t
				    &tv0 = _dthdr.finalverts[0],
				    &tv1 = _dthdr.finalverts[1],
				    &tv2 = _dthdr.finalverts[2]
				;


				_dthdr.prjco[0] = this->_project(proj, tv0.co);
				_dthdr.prjco[1] = this->_project(proj, tv1.co);
				_dthdr.prjco[2] = this->_project(proj, tv2.co);

				//const fpvec3 edt1 = (_dthdr.prjco[1]) - (_dthdr.prjco[0]);
				//const fpvec3 edt2 = (_dthdr.prjco[2]) - (_dthdr.prjco[0]);

				//const fpvec3 vnorm = glm::cross(edt1, edt2);
                const fpvec3 vnorm = glm::cross((_dthdr.vverts[1].co - _dthdr.vverts[0].co), (_dthdr.vverts[2].co - _dthdr.vverts[0].co));
                const fpvec3 facevec = -(_dthdr.vverts[0].co);
				//const bool is_frontface = glm::dot(vnorm, frontvec) >= 0.0;
                const bool is_frontface = glm::dot(vnorm, facevec) >= 0;
				enum_t cullface = this->_rps.en_cull_face;

				if (this->_rps.cap_face_culling && ((cullface == enum_t::FRONT_FACE && is_frontface) || (cullface == enum_t::BACK_FACE && !is_frontface))){
					return; // frontface/backface culling.
				}

				const fpscalar frontface_sign = (is_frontface ? fpscalar(1) : fpscalar(-1));

				const ivec2 xy0 = _math::ndc2xy(oprjco[0], width, height);
				const ivec2 xy1 = _math::ndc2xy(oprjco[1], width, height);
				const ivec2 xy2 = _math::ndc2xy(oprjco[2], width, height);

				aabb2Di& tribox = _dthdr.tribox, *triboxes = &tribox;
				tribox = _math::tri_aabb(xy0, xy1, xy2);
				if (!_math::rect2ds_overlap(tribox, _hdr.screenbounds)){return;}
				_math::clamp_rect(tribox, _hdr.screenbounds);

				const fpmat2 bcmatrix = glm::inverse(fpmat2{
					fpvec2(xy0-xy2), fpvec2(xy1-xy2)
				});


				for (size_t i = 0; i < 3; i++) {
					_dthdr.viewco[i] = _dthdr.finalverts[i].co;
					_dthdr.worldco[i] = fpvec3(_hdr.view * fpvec4(_dthdr.finalverts[i].co, 1));
					_dthdr.worldnormals[i] = glm::normalize(fpvec3(_hdr.mdl * fpvec4(_dthdr.finalverts[i].normal, 0)) * frontface_sign);
					_dthdr.viewnormals[i] = glm::normalize(fpvec3(_hdr.mdlview * fpvec4(_dthdr.finalverts[i].normal, 0)) * frontface_sign);
					_dthdr.zvalues[i] = oprjco[i].z;
					_dthdr.vcolors_u8[i] = _math::fp2u8(_dthdr.finalverts[i].color);
				}

				_tri_barycentric_holder &trilerp2 = _dthdr.trilerp2, &trilerp2pc = _dthdr.trilerp2pc;
				trilerp2 = _tri_barycentric_holder();
				trilerp2pc = _tri_barycentric_holder();
				fpvec3* affine_corners = _dthdr.affine_corners;
				fpvec3* pc_corners = _dthdr.pc_corners;
				fpvec3& pc_wvals = _dthdr.pc_wvals;
				fpvec3& pc_invwvals = _dthdr.pc_invwvals;

				affine_corners[0] = _math::barycentric_calcweights(bcmatrix, tribox[0], xy2);
				affine_corners[1] = _math::barycentric_calcweights(bcmatrix, ivec2{tribox[0].x, tribox[1].y}, xy2);
				affine_corners[2] = _math::barycentric_calcweights(bcmatrix, ivec2{tribox[1].x, tribox[0].y}, xy2);
				affine_corners[3] = _math::barycentric_calcweights(bcmatrix, tribox[1], xy2);

				pc_wvals = fpvec3{oprjco[0].w, oprjco[1].w, oprjco[2].w};
				pc_invwvals = 1.0 / pc_wvals;

				pc_corners[0] = _math::perspective_divide(affine_corners[0], pc_invwvals);
				pc_corners[1] = _math::perspective_divide(affine_corners[1], pc_invwvals);
				pc_corners[2] = _math::perspective_divide(affine_corners[2], pc_invwvals);
				pc_corners[3] = _math::perspective_divide(affine_corners[3], pc_invwvals);

				trilerp2 = affine_corners[0];
				trilerp2pc = pc_corners[0];

				/*
				for (int svno = 0; svno < 3; svno++){
                    auto& sv1 = _dthdr.finalverts[svno];
                    _dthdr.vtangents[svno] = (fpvec3(_hdr.mdl * fpvec4(sv1.tangent, 0)));
                    _dthdr.vbinormals[svno] = (fpvec3(_hdr.mdl * fpvec4(sv1.binormal, 0)));
                    //_dthdr.vupvecs[svno] = glm::normalize(glm::cross(_dthdr.vtangents[svno], _dthdr.vbinormals[svno]));
                    _dthdr.vupvecs[svno] = (fpvec3(_hdr.mdl * fpvec4(sv1.normal, 0)));
                }
                */
				fpscalar &xfincr = _dthdr.xfincr, &yfincr = _dthdr.yfincr;

				xfincr = 1.0/(tribox[1].x - tribox[0].x), yfincr = 1.0/(tribox[1].y - tribox[0].y);

				int &x = _dthdr.x, &y = _dthdr.y;

				for (y = tribox[0].y; y <= tribox[1].y; y++) {
					const auto& rps = this->_rps;
					const fpscalar yfactor = _dthdr.yfactor = (y - tribox[0].y) * yfincr;


					const fpvec3 xlbcw = glm::mix(affine_corners[0], affine_corners[1], yfactor);
					const fpvec3 xrbcw = glm::mix(affine_corners[2], affine_corners[3], yfactor);
					for (x = tribox[0].x; x <= tribox[1].x; x++) {
						const fpscalar xfactor = (x - tribox[0].x) * xfincr;
						const fpvec3 affine_bcw = glm::mix(xlbcw, xrbcw, xfactor);
						trilerp2 = affine_bcw;
						if (trilerp2.outside_tri()){continue;}
						trilerp2pc = _math::perspective_divide(affine_bcw, pc_invwvals);
						this->_sr_drawtri_plotfrag(_dthdr);
					}
				}

			}
			void _sr_drawtri_plotfrag1(_drawtri_header1& _dthdr) {
				const auto& rps = this->_rps;
				const int x = _dthdr.x;
				const int y = _dthdr.y;
				const fpvec4* oprjco = _dthdr.prjco;

				fpscalar *zptr = this->curfbo->depthptr(x, y);
				fpscalar z = _dthdr.trilerp2.lerpF(_dthdr.zvalues[0], _dthdr.zvalues[1], _dthdr.zvalues[2]);
				z *= glm::abs(z);
				if (rps.cap_depth_test && ((z < -1) || (z > 1) || (!_math::valcmp(rps.en_depth_func, z, *zptr)))){
					return;
				}

				const ivec2 xy(x, y);

				if (rps.cap_draw_depth_buffer){*zptr = z;}

				//const u8rgba vcolor = _dthdr.trilerp2pc.lerpU8(_dthdr.vcolors_u8);
				const u8rgba _vcolors1[3] = {
					{255,0,0,255},
					{0,255,0,255},
					{0,0,255,255}
				};
				const u8rgba vcolor = _dthdr.trilerp2pc.lerpU8(_vcolors1);

				this->_sr_writecolor(xy, vcolor);
			}



			void _sr_drawtri_plotfrag(_drawtri_header1& _dthdr) {
				const auto& rps = this->_rps;
				auto& _hdr = *_dthdr._hdrptr;
				const auto& cmat = *_hdr.cmatptr;

				const int x = _dthdr.x;
				const int y = _dthdr.y;
				const fpvec4* oprjco = _dthdr.prjco;

				fpscalar *zptr = this->curfbo->depthptr(x, y);
				fpscalar z = _dthdr.trilerp2.lerpF(_dthdr.zvalues[0], _dthdr.zvalues[1], _dthdr.zvalues[2]);
				//z *= glm::abs(z);
				if (rps.cap_depth_test && ((z < -1) || (z > 1) || (!_math::valcmp(rps.en_depth_func, z, *zptr)))){
					return;
				}

				const ivec2 xy(x, y);
				this->_sr_drawtri_plotfrag_basic_setup1(_dthdr);

                //_dthdr.tangent_vector = glm::normalize(_dthdr.trilerp2pc.lerpF(_dthdr.vtangents));
                //_dthdr.binormal_vector = glm::normalize(_dthdr.trilerp2pc.lerpF(_dthdr.vbinormals));
                //_dthdr.up_vector = glm::normalize(_dthdr.trilerp2pc.lerpF(_dthdr.vupvecs));

				_sr_texturing_result texres = {};
				texres.diffuse_color = fpvec3(_dthdr.frag_base_diffuse);
				texres.diffuse_alpha = _dthdr.frag_base_diffuse[3];
				texres.specular_color = fpvec3(cmat.base_specular_color);
				texres.specular_alpha = cmat.base_specular_color[3];
				texres.diffuse_intensity = cmat.base_diffuse_intensity;
				texres.specular_intensity = cmat.base_specular_intensity;
				texres.specular_hardness = cmat.base_specular_hardness;

				for (size_t i = 0; i < _hdr.ntexslots; i++) {
					const MaterialTextureSlot& mts = _hdr.texslots[i];
					this->_sr_applytexture(_dthdr, texres, mts);
				}

				fpvec4 fragcolor = this->_sr_drawtri_plotfrag_process_lighting(
				    _dthdr,
				    texres.diffuse_intensity,
				    texres.specular_intensity,
				    texres.specular_hardness,
				    fpvec4{0,0,0,texres.diffuse_alpha}, fpvec4(texres.diffuse_color, texres.diffuse_alpha),
				    fpvec4{0,0,0,0}, fpvec4(texres.specular_color, 1)
				);

				if (fragcolor[3] < cmat.alpha_threshold) {return;}
				else if (this->_rps.cap_blending && cmat.uses_transparency)
				{
					/*
					const u8rgba dst = _math::fp2u8(fragcolor);
					const u8rgba src = get_color_fromptr(this->curfbo->colorptr(x, y), this->curfbo->rbuf_fmt);
					const uint8_t alpha = dst[3];
					this->_sr_writecolor(xy, u8rgba(_math::u8blend(cmat.blendtype, u8rgb(src), u8rgb(dst), u8rgb(alpha)), dst[3]));
					*/
					const fpvec4 src = _math::u8tofp(get_color_fromptr(this->curfbo->colorptr(x,y), this->curfbo->rbuf_fmt));
					this->_sr_writecolor(xy, u8rgba(_math::fp2u8(_math::blend(cmat.blendtype, fpvec3(src), fpvec3(fragcolor), fpvec3(fragcolor[3]))), _math::fp2u8(fragcolor[3])));
				}
				else
				{
					this->_sr_writecolor(xy, _math::fp2u8(fragcolor));
				}
				if (rps.cap_draw_depth_buffer){*zptr = z;}
				//this->_sr_writecolor(xy, fragcolor);
			}
					//CUT!!!

					/*
					const fpvec4 oprjco[3] = {
						this->_project(_hdr.mvp, v0.co),
						this->_project(_hdr.mvp, v1.co),
						this->_project(_hdr.mvp, v2.co)
					};



					this->_tri_num_splits = 1;
					size_t num_behind_near = 0;

					if (oprjco[0].w < 0){num_behind_near++;}
					if (oprjco[1].w < 0){num_behind_near++;}
					if (oprjco[2].w < 0){num_behind_near++;}

					if (num_behind_near > 0){continue;} //we'll just discard any triangles that are bisected by the W = 0 plane for now.

					const ivec2 xy0 = _math::ndc2xy(oprjco[0], width, height);
					const ivec2 xy1 = _math::ndc2xy(oprjco[1], width, height);
					const ivec2 xy2 = _math::ndc2xy(oprjco[2], width, height);

					_math::tri_aabb(triboxes[0], xy0, xy1, xy2);
					if (!_math::rect2ds_overlap(triboxes[0], _hdr.screenbounds)){continue;}
					_math::clamp_rect(triboxes[0], _hdr.screenbounds);

					fpmat2 bcmatrix = glm::inverse(fpmat2{
						fpvec2(xy0-xy2), fpvec2(xy1-xy2)
					});

					const fpvec3 clipnormal = glm::cross(fpvec3(oprjco[1])-fpvec3(oprjco[0]), fpvec3(oprjco[2])-fpvec3(oprjco[0]));
					const bool is_frontface = (glm::dot(clipnormal, fpvec3{0,0,1}) > 0.0);
					if (this->_rps.cap_face_culling)
					{
						if ((this->_rps.en_cull_face == enum_t::FRONT_FACE && is_frontface) || (this->_rps.en_cull_face == enum_t::BACK_FACE && !is_frontface))
						{
							continue;
						}
					}

					const fpvec3 worldco[3] = {
						fpvec3(_hdr.mdl*fpvec4(v0.co, 1.0)),
						fpvec3(_hdr.mdl*fpvec4(v1.co, 1.0)),
						fpvec3(_hdr.mdl*fpvec4(v2.co, 1.0))
					};

					const fpvec3 viewco[3] = {
						fpvec3(_hdr.mdlview*fpvec4(v0.co, 1)),
						fpvec3(_hdr.mdlview*fpvec4(v1.co, 1)),
						fpvec3(_hdr.mdlview*fpvec4(v2.co, 1))
					};

					const fpscalar zvalues[3] = {
						oprjco[0].z,
						oprjco[1].z,
						oprjco[2].z
					};

					const u8rgba vcolors_u8[3] = {
						_math::fp2u8(v0.color),
						_math::fp2u8(v1.color),
						_math::fp2u8(v2.color)
					};

					const fpvec3 worldnormals[3] = {
						glm::normalize(fpvec3(_hdr.mdl*fpvec4(v0.normal, 0))),
						glm::normalize(fpvec3(_hdr.mdl*fpvec4(v1.normal, 0))),
						glm::normalize(fpvec3(_hdr.mdl*fpvec4(v2.normal, 0)))
					};

					const fpvec3 viewnormals[3] = {
						glm::normalize(fpvec3(_hdr.mdlview*fpvec4(v0.normal, 0))),
						glm::normalize(fpvec3(_hdr.mdlview*fpvec4(v1.normal, 0))),
						glm::normalize(fpvec3(_hdr.mdlview*fpvec4(v2.normal, 0)))
					};
					for (j = 0; j < this->_tri_num_splits; j++)
					{
						const aabb2Di tribox = triboxes[j];

						#ifndef DJ_SW3D20_USE_OPENMP
							_math::tri2d_interpolator& trilerp = _basetrilerps[j];
						#endif
						#ifdef DJ_SW3D20_USE_OPENMP
						 #pragma omp parallel for simd schedule(runtime)
						#endif

						_tri_barycentric_holder trilerp2, trilerp2pc;
						const fpvec3 affine_corners[4] = {
							_math::barycentric_calcweights(bcmatrix, tribox[0], xy2),
							_math::barycentric_calcweights(bcmatrix, ivec2{tribox[0].x, tribox[1].y}, xy2),
							_math::barycentric_calcweights(bcmatrix, ivec2{tribox[1].x, tribox[0].y}, xy2),
							_math::barycentric_calcweights(bcmatrix, tribox[1], xy2)
						};

						const fpvec3 pc_wvals = {oprjco[0].w, oprjco[1].w, oprjco[2].w};
						const fpvec3 pc_invwvals = 1.0 / pc_wvals;

						const fpvec3 pc_corners[4] = {
							_math::perspective_divide(affine_corners[0], pc_invwvals),
							_math::perspective_divide(affine_corners[1], pc_invwvals),
							_math::perspective_divide(affine_corners[2], pc_invwvals),
							_math::perspective_divide(affine_corners[3], pc_invwvals)
						};
						trilerp2 = affine_corners[0];
						trilerp2pc = pc_corners[0];
						const fpscalar xfincr = 1.0/(tribox[1].x - tribox[0].x), yfincr = 1.0/(tribox[1].y - tribox[0].y);

						for (int y = tribox[0].y; y <= tribox[1].y; y++)
						{
							const auto& rps = this->_rps;
							const fpscalar yfactor = (y - tribox[0].y) * yfincr;
							const fpvec3 xlbcw = glm::mix(affine_corners[0], affine_corners[1], yfactor);
							const fpvec3 xrbcw = glm::mix(affine_corners[2], affine_corners[3], yfactor);
							for (int x = tribox[0].x; x <= tribox[1].x; x++)
							{
								const fpscalar xfactor = (x - tribox[0].x) * xfincr;
								const fpvec3 affine_bcw = glm::mix(xlbcw, xrbcw, xfactor);
								trilerp2 = affine_bcw;

								if (trilerp2.outside_tri()){continue;}

								trilerp2pc = _math::perspective_divide(affine_bcw, pc_invwvals);
								fpscalar *zptr = this->curfbo->depthptr(x, y);
								fpscalar z = trilerp2.lerpF(oprjco[0].z, oprjco[1].z, oprjco[2].z);
								z *= glm::abs(z);
								if (rps.cap_depth_test)
								{
									if ((z < -1) || (z > 1) || (!_math::valcmp(rps.en_depth_func, z, *zptr))){continue;}
								}

								const ivec2 xy(x, y);

								if (rps.cap_draw_depth_buffer){*zptr = z;}
								const fpvec3 fragpos_world = (_hdr.must_calc_fragpos_world ? trilerp2pc.lerpF(worldco) : _hdr.campos+fpvec3{0,1,0});
								const fpvec3 fragpos_view = (_hdr.must_calc_fragpos_view ? trilerp2pc.lerpF(viewco) : fpvec3{0,0,-1});
								const fpvec3 viewvec_w = (_hdr.must_calc_viewvec_w ? glm::normalize(fragpos_world-_hdr.campos) : fpvec3{0,1,0});
								const fpvec3 viewvec = (_hdr.must_calc_viewvec ? glm::normalize(fragpos_view) : fpvec3{0,0,-1});
								const fpvec3 worldnorm = (_hdr.must_calc_worldnormal ? glm::normalize(trilerp2pc.lerpF(worldnormals)) : fpvec3{0,0,1});
								const fpvec3 viewnorm = (_hdr.must_calc_viewnormal ? glm::normalize(trilerp2pc.lerpF(viewnormals)) : fpvec3{0,0,1});

								const fpvec3 mirrorball_refl = (_hdr.must_calc_mirrorball ? glm::reflect(viewvec, viewnorm) : fpvec3{0,0,0});

								const fpvec2 mirrorball_uv = {0.5 + (0.4 * mirrorball_refl.x), 0.5 + (0.4 * mirrorball_refl.y)};

								const fpvec3 greflvec = (_hdr.must_calc_greflvec ? glm::reflect(viewvec_w, worldnorm) : fpvec3{0,0,1});

								const u8rgba vcolor = trilerp2pc.lerpU8(vcolors_u8);
								const fpscalar lod = 0.5 + (0.5 * z);

								u8rgba base_diffuse = _hdr.base_diffuse_color;
								if (cmat.vertex_color_paint.use){
									base_diffuse = _math::u8lerp(base_diffuse, vcolor, _hdr.vcp_u8);
								}
								if (cmat.vertex_color_light.use){
									base_diffuse = _math::u8blend(cmat.vertex_color_light_blend, base_diffuse, vcolor, _hdr.vcl_u8);
								}

								u8rgb dcolor(base_diffuse);
								uint8_t balpha = base_diffuse[3];
								u8rgb scolor(_hdr.base_specular_color);
								for (size_t tsi = 0; tsi < _hdr.ntexslots; tsi++)
								{
									const MaterialTextureSlot& mts = _hdr.texslots[tsi];
									if (!mts.use){continue;}
									u8rgba texel = {0,0,0,0};
									switch (mts.textype)
									{
										case enum_t::TEXTYPE_IMAGE:
										{
											if (mts.src_image != nullptr)
											{
												switch (mts.mapping_type)
												{
													case enum_t::TEXMAP_UVMAP:
													{
														if (mts.uvmapno >= 0 && mts.uvmapno < 16)
														{
															const fpvec2 uv = trilerp2pc.lerpF(v0.uv[mts.uvmapno], v1.uv[mts.uvmapno], v2.uv[mts.uvmapno]);
															texel = mts.src_image->sample2D(uv, lod);
														}
														break;
													}
													case enum_t::TEXMAP_REFLECTION:
													{
														texel = mts.src_image->sample2D(mirrorball_uv, lod);
														break;
													}
													case enum_t::TEXMAP_SCREEN_01:
													{
														texel = mts.src_image->sample2D(fpvec2{fpscalar(x)/(width-1), fpscalar(y)/(height-1)}, lod);
														break;
													}
													default: break;
												}
											}
											break;
										}
										case enum_t::TEXTYPE_ENVMAP:
										{
											if (mts.src_envmap != nullptr)
											{
												switch (mts.mapping_type)
												{
													case enum_t::TEXMAP_VIEWVECTOR:
													{
														switch (mts.envmaptype)
														{
															case enum_t::ENVMAP_EQUIRECT: texel = mts.src_envmap->sampleEquirect(viewvec_w, lod); break;
															default: break;
														}
														break;
													}
													case enum_t::TEXMAP_REFLECTION:
													{
														switch (mts.envmaptype)
														{
															case enum_t::ENVMAP_EQUIRECT: texel = mts.src_envmap->sampleEquirect(greflvec, lod); break;
															default: break;
														}
														break;
													}
													default: break;
												}
											}
											break;
										}
										default: break;
									}
									if (mts.affects_diffuse_color.use)
									{
										dcolor = (_math::u8blend(mts.layerblendtype, dcolor, u8rgb(texel), _math::fp2u8(mts.affects_diffuse_color.amount)));
									}
									if (mts.affects_diffuse_alpha.use)
									{
										balpha = _math::u8blend(mts.layerblendtype, u8ra(balpha), u8ra(texel[mts.alpha_channel]), u8ra(_math::fp2u8(mts.affects_diffuse_alpha.amount)))[0];
									}
								}
								const u8rgba fragcolor(dcolor, balpha);

								if (this->_rps.cap_blending && cmat.uses_transparency)
								{
									const u8rgba dst = fragcolor;
									const u8rgba src = get_color_fromptr(this->curfbo->colorptr(x, y), this->curfbo->rbuf_fmt);
									const uint8_t alpha = dst[3];
									this->_sr_writecolor(xy, u8rgba(_math::u8blend(cmat.blendtype, u8rgb(src), u8rgb(dst), u8rgb(alpha)), dst[3]));
								}
								else
								{
									this->_sr_writecolor(xy, fragcolor);
								}
							}
						}
					}
				}
				*/

			void srDrawElements(const ffvertexlist_t& ffvlist)
			{
			    this->srDrawElements(ffvlist.size(), ffvlist.data());
			}

			void srDrawUnitRectXY()
			{
				const ffvertex_t urect[6] = {
					{.co={-0.5,-0.5,0}, .normal={0,0,1}, .uv={ {0,0} }}, {.co={0.5,-0.5,0}, .normal={0,0,1}, .uv={ {1,0} }}, {.co={-0.5,0.5,0}, .normal={0,0,1}, .uv={ {0,1} }},
					{.co={-0.5,0.5,0}, .normal={0,0,1}, .uv={ {0,1} }}, {.co={0.5,-0.5,0}, .normal={0,0,1}, .uv={ {1,0} }}, {.co={0.5,0.5,0}, .normal={0,0,1}, .uv={ {1,1} }}
				};

				this->srDrawElements(6, urect);
			}

			int srDrawTextQ3A(const std::string str, Texture* font, const fpvec4 basecolor={1,1,1,1})
			{
				int num = 0;
			    ffvertex_t urect[6] = {
					{.co={-0.5,-0.5,0}, .uv={ {0,0} }}, {.co={0.5,-0.5,0}, .uv={ {1,0} }}, {.co={-0.5,0.5,0}, .uv={ {0,1} }},
					{.co={-0.5,0.5,0}, .uv={ {0,1} }}, {.co={0.5,-0.5,0}, .uv={ {1,0} }}, {.co={0.5,0.5,0}, .uv={ {1,1} }}
				};
				this->_q3bigchar_material.base_diffuse_color = basecolor;
				this->_q3bigchar_material.texture_slots[0].src_image = font;
				auto oldmat = this->srActiveMaterial(&(this->_q3bigchar_material));
				const size_t nchrs = str.size();

				int col = 0, line = 0;

				const int fontw = font->width, fonth = font->height;
				const fpvec2 originuv = {0, 1}, charspan = {1.0/16.0, -1.0/16.0}, pixelspan = {1.0/(fontw-1), -1.0/(fonth-1)}, uvstep = charspan+pixelspan;
				const uint8_t* ords = (const uint8_t*)(str.c_str());
				auto oldmm = this->srMatrixMode(enum_t::MODEL_MATRIX);
				this->srPushMatrix();
				for (size_t i = 0; i < nchrs; i++)
				{
					const uint8_t chr = ords[i];

			    	if (chr == '\r' || chr == '\n') //don't draw newlines.
			    	{
			    	    col = 0; line++;
			    		if (chr == '\r' && (i+1) < nchrs && ords[i+1] == '\n'){i++;} //treats a CR + LF (DOS/Windows newline sequence) as one newline instead of two.
			    	}
			    	else if (chr == ' '){col++;} //don't draw spaces either.
			    	else
			    	{
			    	    this->srReloadParentMatrix();
    					this->srTranslatef(0.5, 0.5, 0.0);
    				    this->srTranslatef((col*1.0125), (line*-1.0125), 0.0);

			    		const uint bcrow = uint(chr) / 16U, bccol = uint(chr) % 16U;
			    		const fpvec2 tluv = originuv + (charspan * fpvec2{bccol, bcrow});

			    		const fpvec2 truv = tluv + fpvec2{charspan.x, 0.0},
			    		             bluv = tluv + fpvec2{0.0, charspan.y}, bruv = tluv + charspan
			    		;

			    		urect[0].uv[0] = bluv;
			    		urect[1].uv[0] = bruv; urect[4].uv[0] = bruv;
			    		urect[2].uv[0] = tluv; urect[3].uv[0] = tluv;
			    		urect[5].uv[0] = truv;

			    		this->srDrawElements(6, urect);

			    		num++;
			    		col++;
			    	}
				}
				this->srPopMatrix();
				this->srMatrixMode(oldmm);
				this->srActiveMaterial(oldmat);
				return num;
			}

			fpvec3 srViewDirNDC(const fpvec3& ndc, const bool world=true)
			{
				const fpvec3 csray(glm::inverse(fpmat4(this->projection_matrix))*fpvec4(ndc, 1.0));
				if (world){return glm::normalize(fpvec3(fpmat4(this->view_matrix)*fpvec4(csray, 0.0)));}
				return glm::normalize(csray);
			}
			fpvec3 srViewDirXY(const fpvec2& xy, const bool world=true)
			{
				const fpvec3 csray(glm::inverse(fpmat4(this->projection_matrix))*fpvec4{-1.0 + (2.0*xy.x*(1.0/this->curfbo->width-1)), -1.0 + (2.0*xy.y*(1.0/this->curfbo->height-1)), 1.0, 1.0});
				if (world){return glm::normalize(fpvec3(fpmat4(this->view_matrix)*fpvec4(csray, 0.0)));}
				return glm::normalize(csray);
			}
			Context() {
				#ifdef DJ_SW3D20_USE_OPENMP
				    this->_maxnthreads = omp_get_max_threads();
				#else
				    this->_maxnthreads = 1;
				#endif
				std::cout << "Max # of OpenMP threads = " << this->_maxnthreads << '\n';
				//this->_trilerps[0].resize(this->_maxnthreads);
				//this->_trilerps[1].resize(this->_maxnthreads);
				this->_trilerps[0] = new _math::tri2d_interpolator[this->_maxnthreads];
				this->_trilerps[1] = new _math::tri2d_interpolator[this->_maxnthreads];
			}
			~Context() {
				this->srContextShutdown();
				delete[] this->_trilerps[0];
				delete[] this->_trilerps[1];
			}

	};

	namespace aux {
		#ifndef DJ_SW3D20_STBI_NAME
		 #define DJ_SW3D20_STBI_NAME(_n) _n
		#endif

		#ifndef DJ_SW3D20_STBIW_NAME
		 #define DJ_SW3D20_STBIW_NAME(_n) _n
		#endif
		#ifdef INCLUDE_STB_IMAGE_WRITE_H
		    void stbi_rbuf_write_png(const std::string filename, Framebuffer* fbo)
		    {
		    	if (fbo == nullptr){throw std::runtime_error("null FBO!");}
		    	else if (fbo->renderbuffer == nullptr){throw std::runtime_error("null renderbuffer!");}
		    	DJ_SW3D20_STBIW_NAME(stbi_write_png)(filename.c_str(), fbo->width, fbo->height, fbo->rbuf_nch, fbo->renderbuffer, (fbo->width) * (fbo->rbuf_nch));
		    }
		#endif
		#ifdef STBI_INCLUDE_STB_IMAGE_H
		    using _aux_stbi_iocbs_t = DJ_SW3D20_STBI_NAME(stbi_io_callbacks);
		    /*
		    int _aux_stbi_iocb_read(void* user, char* data, int size);
		    void _aux_stbi_iocb_skip(void* user, int n);
		    int _aux_stbi_iocb_eof(void* user);
		    class STBI_BaseIOWrapper {
				private:
				    _aux_stbi_iocbs_t _iocbs = {
						.read=dj_sw3d20::aux::_aux_stbi_iocb_read,
						.skip=dj_sw3d20::aux::_aux_stbi_iocb_skip,
						.eof=dj_sw3d20::aux::_aux_stbi_iocb_eof
					};
				public:
				    operator _aux_stbi_iocbs_t*() const {
						return &(this->_iocbs);
					}
					virtual int read(char* data, int size);
					virtual void skip(int n);
					virtual bool eof();
			};

			class STBI_IStreamWrapper : public STBI_BaseIOWrapper {
				public:
				    std::istream* istr = nullptr;

				    int read(char* data, int size)
				    {
						return this->istr->read(data, size).gcount();
					}
					void skip(int n)
					{
						this->istr->seekg(n, std::ios_base::cur);
					}
					bool eof()
					{
						returh (this->istr == nullptr || this->istr->eof());
					}

			};
			*/
			int _aux_stbi_istream_read(void* user, char* data, int size)
			{
				std::istream& istr = *(std::istream*)user;
				return istr.read(data, size).gcount();
			}
			void _aux_stbi_istream_skip(void* user, int n)
			{
				((std::istream*)user)->seekg(n, std::ios_base::cur);
			}
			int _aux_stbi_istream_eof(void* user)
			{
				return ((std::istream*)user)->eof();
			}

			Texture* stbi_load_tex2D(const _aux_stbi_iocbs_t& callbacks, void* user, Texture* tex, const int level=0)
			{
				if (tex == nullptr){throw std::runtime_error("tex cannot be nullptr");}
				int w, h, nc;
				uint8_t* px = DJ_SW3D20_STBI_NAME(stbi_load_from_callbacks)((_aux_stbi_iocbs_t*)(&callbacks), user, &w, &h, &nc, 0);
				if (px == nullptr)
				{
					throw std::runtime_error(DJ_SW3D20_STBI_NAME(stbi_failure_reason)());
				}
				tex->texImage2D(px, w, h, enum_t(nc), level, true);
				DJ_SW3D20_STBI_NAME(stbi_image_free)(px);
				px = nullptr;
				return tex;
			}
			Texture* stbi_load_tex2D(std::istream& istr, Texture* tex, const int level=0)
			{
				_aux_stbi_iocbs_t callbacks = {
					.read=_aux_stbi_istream_read,
					.skip=_aux_stbi_istream_skip,
					.eof=_aux_stbi_istream_eof
				};
				return stbi_load_tex2D(callbacks, (void*)&istr, tex, level);
			}

			Texture* stbi_load_tex2D(const std::string filename, Texture* tex, const int level=0)
			{
				std::fstream ifs(filename, std::ios::in | std::ios::binary);
				if (!ifs){throw std::runtime_error(std::string("Can't open filename for reading: ")+filename);}
			    stbi_load_tex2D(ifs, tex, level);
			    ifs.close();
			    return tex;
			}
		#endif
		#ifdef DJ_SW3D20_AUX_HAS_HAPPLY
		template <class T>
		std::vector<T> _happly_getprop(happly::Element& e, const std::string name, T defaultfill=T())
		{
			if (e.hasProperty(name)){return e.getProperty<T>(name);}
			else
			{
			    std::vector<T> fb; fb.resize(e.count, defaultfill);
			    return fb;
			}
		}
		void ply2ffvs(happly::PLYData& ply, ffvertexlist_t& ffvlist)
		{
			happly::Element
			    &evertex = ply.getElement("vertex"),
			    &eface = ply.getElement("face")
			;

			auto px = _happly_getprop<fpscalar>(evertex, "x");
			auto py = _happly_getprop<fpscalar>(evertex, "y");
			auto pz = _happly_getprop<fpscalar>(evertex, "z");

			auto pred = _happly_getprop<uint8_t>(evertex, "red", 255);
			auto pgreen = _happly_getprop<uint8_t>(evertex, "green", 255);
			auto pblue = _happly_getprop<uint8_t>(evertex, "blue", 255);

			auto pnx = _happly_getprop<fpscalar>(evertex, "nx");
			auto pny = _happly_getprop<fpscalar>(evertex, "ny");
			auto pnz = _happly_getprop<fpscalar>(evertex, "nz");

			//auto pred = evertex.getProperty<uint8_t>("red");
			//auto pgreen = evertex.getProperty<uint8_t>("green");
			//auto pblue = evertex.getProperty<uint8_t>("blue");

			//auto pnx = evertex.getProperty<fpscalar>("nx");
			//auto pny = evertex.getProperty<fpscalar>("ny");
			//auto pnz = evertex.getProperty<fpscalar>("nz");

			auto ps = _happly_getprop<fpscalar>(evertex, "s");
			auto pt = _happly_getprop<fpscalar>(evertex, "t");

			auto pvi = eface.getListProperty<size_t>("vertex_indices");

			const size_t nfaces = eface.count;
			const size_t nverts = evertex.count;

			for (auto& indseq : pvi)
			{
				if (indseq.size() != 3){throw std::runtime_error("non-tri encountered in PLY!");}
				for (auto fi : indseq)
				{
				    ffvertex_t newvert = {
						.co=fpvec3{px[fi], py[fi], pz[fi]},
						.normal=fpvec3{pnx[fi], pny[fi], pnz[fi]},
						//.color={0.5+(0.5*pnx[fi]), 0.5+(0.5*pny[fi]), 0.5+(0.5*pnz[fi]), 1.0}
						//.color={fpscalar(pred[fi])/255, fpscalar(pgreen[fi])/255, fpscalar(pblue[fi])/255, 1.0}
						.color=fpvec4{pred[fi]/255.0, pgreen[fi]/255.0, pblue[fi]/255.0, 1.0}
					};
					newvert.uv[0] = fpvec2{ps[fi], pt[fi]};

					ffvlist.push_back(newvert);
				}
			}
		}

		void ply2ffvs(const std::string filename, ffvertexlist_t& ffvlist)
		{
			happly::PLYData ply(filename, true);
			ply2ffvs(ply, ffvlist);
		}
		#endif
	};
}
#endif
