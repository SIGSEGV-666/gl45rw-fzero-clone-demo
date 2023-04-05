#ifndef DJUTIL_H_INCLUDED
#define DJUTIL_H_INCLUDED

#include <algorithm>
#include <iterator>
#include <functional>
#include <vector>
#include <unordered_map>
#include <exception>

//#include <features.h>

#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <streambuf>

#include <cstring>
#include <cstdio>


#include <string>
#include <cmath>

#include <limits>
#include <typeinfo>
#include <type_traits>

//begin forward decls.
namespace djutil {
    namespace containers {};
    namespace fixedpoint {};
    namespace linalg {};
	namespace ezstr {};
	namespace binio {};
	namespace vircon {};
	namespace ndarray {};
	namespace imaging {};
	namespace dmd {};
	namespace uvm {};
	namespace platform {};
	namespace ufpio {};
	namespace pathman {};
	namespace virfs {};
};
//end forward decls.
#endif

#if (defined(DJUTIL_NEEDS_virfs) && !defined(__DJUTIL_H_pathman_LOADED))
    #define DJUTIL_NEEDS_pathman
    #define DJUTIL_NEEDS_ufpio
#endif

#if (defined(DJUTIL_NEEDS_ufpio))
    #define DJUTIL_NEEDS_ezstr
    #define DJUTIL_NEEDS_platform
#endif

#ifdef DJUTIL_NEEDS_uvm
    #define DJUTIL_NEEDS_binio
    #define DJUTIL_NEEDS_fixedpoint
#endif

#ifdef DJUTIL_NEEDS_ndarray
    #define DJUTIL_NEEDS_binio
#endif

#ifdef DJUTIL_NEEDS_vircon
    #define DJUTIL_NEEDS_ezstr
#endif

#ifdef DJUTIL_NEEDS_dmd
    #define DJUTIL_NEEDS_binio
#endif

#ifdef DJUTIL_NEEDS_imaging
    #define DJUTIL_NEEDS_ezstr
    #define DJUTIL_NEEDS_binio
    #define DJUTIL_NEEDS_linalg
#endif

#ifdef DJUTIL_NEEDS_linalg
    #define DJUTIL_NEEDS_fixedpoint
#endif

#ifdef DJUTIL_NEEDS_virfs
    #define DJUTIL_NEEDS_binio
    #define DJUTIL_NEEDS_ezstr
    #define DJUTIL_NEEDS_containers
#endif

#ifdef DJUTIL_NEEDS_pathman
    #define DJUTIL_NEEDS_ezstr
    #define DJUTIL_NEEDS_platform
    #define DJUTIL_NEEDS_containers
#endif

#ifdef DJUTIL_NEEDS_platform
    #define DJUTIL_NEEDS_ezstr
#endif

#if (defined(DJUTIL_NEEDS_containers) && !defined(__DJUTIL_H_containers_LOADED))
#define __DJUTIL_H_containers_LOADED
namespace djutil {namespace containers{
	#include <algorithm>
	#include <vector>
	#include <unordered_map>
	#include <iostream>
	#include <string>

	static const size_t npos = -1;
	/*
	struct null_ostream_writer {
		template <class VT>
		static void writer(std::ostream& os, const VT& v) {
			os << '?';
		}
	};

	struct std_ostream_writer {
		template <class VT>
		static void writer(std::ostream& os, const VT& v) {
			os << v;
		}
	};
	*/

	void _write_value_to(std::ostream& ostr, const std::string& value)
	{
		ostr << std::quoted(value);
	}

    template <class T>
    void _write_value_to(std::ostream& ostr, const T& value) {ostr << value;}


	template <class E>
	class vectorlist : public std::vector<E> {
	    using std::vector<E>::vector;

	    public:
	        using base_type = std::vector<E>;
	        using iterator = typename base_type::iterator;
	        using const_iterator = typename base_type::const_iterator;
	        inline const_iterator iterToFirstOf(const E& e) const {
	        	return std::find(this->cbegin(), this->cend(), e);
	        }
	        inline iterator iterToFirstOf(const E& e) {
	        	return std::find(this->begin(), this->end(), e);
	        }

	        size_t firstIndexOf(const E& e) const {
				const_iterator ib = this->cbegin(), ie = this->cend();
	        	const_iterator r = std::find(ib, ie, e);
	        	return (r < ie ? size_t(r-ib) : djutil::containers::npos);
	        }

	        bool eraseFirstOf(const E& e)
	        {
	        	iterator r = this->iterToFirstOf(e);
	        	if (r != this->end()){this->erase(r); return true;}
	        	else {return false;}
	        }
	        bool eraseElemAtIndex(const size_t& idx) {
				if (idx >= this->size()){return false;}
				this->erase(this->begin()+idx);
				return true;
			}
	        size_t eraseAllOf(const E& e)
	        {
	            size_t _count = 0;
	        	while (this->eraseFirstOf(e)){_count++;}
	        	return _count;
	        }

	        inline size_t sizeBytes() const {
	        	return this->size() * sizeof(E);
	        }

	        inline const E* pfront() const {return (this->size() > 0 ? &this->front() : nullptr);}
	        inline const E* pback() const {return (this->size() > 0 ? &this->back() : nullptr);}

	        inline E* pfront() {return (this->size() > 0 ? &this->front() : nullptr);}
	        inline E* pback() {return (this->size() > 0 ? &this->back() : nullptr);}



	        friend std::ostream& operator<<(std::ostream& os, const vectorlist<E>& self) {
   	            os << '{';
   	            for (size_t i = 0, j = 1; i < self.size(); i++, j++){
   	            	//djutil::containers::_choice_writer<OSWF>::template write<E>(os, self[i]);
   	            	_write_value_to(os, self[i]);
   	            	if (j < self.size()){os << ", ";}
   	            }
   	            os << '}';
   	            return os;
   	        }


	};

	template <class K, class V>
	class dictionary : public std::unordered_map<K,V> {
		public:
		    using std::unordered_map<K,V>::unordered_map;
		    using std::unordered_map<K,V>::operator[];

		    using base_type = std::unordered_map<K,V>;
		    bool getv(V& out, const K& key) const {
		    	if (this->count(key) > 0){
		    	    out = this->at(key);
		    	    return true;
		    	}
		    	return false;
		    }

		    bool popk(const K& key) {
		    	if (this->count(key) > 0){
		    		this->erase(key);
		    		return true;
		    	}
		    	return false;
		    }

		    const V& getd(const K& key, const V& fallback) const {
		        if (this->count(key) > 0){
		        	return this->at(key);
		        }
		        return fallback;
		    }

		    V& setdefault(const K& key, const V& fallback) {
		        const bool setnu = this->count(key) == 0;
		    	if (setnu){return this->insert({key, fallback}).first->second;}
		    	else {return (*this)[key];}
		    }

		    template <class ITR>
		    size_t setpairs(const ITR start, const ITR terminator)
		    {
		        size_t _count = 0;
		    	for (ITR i = start; i != terminator; i++, _count++)
		    	{
		    		(*this)[i->first] = i->second;
		    	}
		    	return _count;
		    }
		    size_t setpairs_v(const std::vector<std::pair<K,V>>& pv)
		    {
		    	return this->setpairs(pv.cbegin(), pv.cend());
		    }
	};
	template <class DT>
	struct treenode {
		using this_type = treenode<DT>;
		using data_type = DT;
		using nodelist_type = vectorlist<treenode<DT>*>;
		private:
		    size_t _maxparents = 1, _maxchildren = -1;
		    nodelist_type _parentnodes = {}, _childnodes = {};

		public:
		    const size_t& max_num_parents = _maxparents;
		    const size_t& max_num_children = _maxchildren;
		    const nodelist_type& parents = _parentnodes;
		    const nodelist_type& children = _childnodes;

		    data_type data;
		    bool rmchild(this_type* child) {
				return (
				    child != nullptr &&
				    this->_childnodes.eraseFirstOf(child) &&
				    child->_parentnodes.eraseFirstOf(this)
				);
			}
			bool addchild(this_type* newchild) {
				if (newchild == nullptr){return false;}
				if (newchild->_parentnodes.size() >= newchild->_maxparents){return false;}
				if (this->_childnodes.size() >= this->_maxchildren){return false;}
				if (newchild->_parentnodes.firstIndexOf(this) != npos){return false;}
				if (this->_childnodes.firstIndexOf(newchild) != npos){return false;}
				this->_childnodes.push_back(newchild);
				newchild->_parentnodes.push_back(this);
				return true;
			}
			template <class P, class L>
			size_t find_childnodes(L& results, const P& predicate, const size_t maxresults=-1) {
				size_t num = 0;
				for (auto it = this->_childnodes.begin(); it != this->_childnodes.end() && num < maxresults; it++) {
					if (predicate(*it)){num++; results.push_back(*it);}
				}
				return num;
			}

		    treenode(const size_t maxchildren=-1, const size_t maxparents=1) : _maxchildren(maxchildren), _maxparents(maxparents) {}
		    ~treenode() {
				//std::cout << "delnode: " << (void*)this << '\n';
				while (this->_parentnodes.size() > 0) {
					auto* pn = this->_parentnodes.back();
					pn->rmchild(this);
				}
				while (this->_childnodes.size() > 0) {
					auto* cn = this->_childnodes.back();
					this->rmchild(cn);
					delete cn; cn = nullptr;
				}
			}
	};
	template <size_t ND, class PDT, class VT, class VAL2AABB, class IDX=int>
	class cellmap {
		using pos_type = std::array<PDT, ND>;
		using aabb_type = std::array<std::array<PDT, ND>, 2>;
		using posdim_type = PDT;
		using value_type = VT;
		using ndidx_type = std::array<IDX, ND>;

		private:
			struct _cell {
				;
			};
	};
}};
#endif
#if (defined(DJUTIL_NEEDS_fixedpoint) && !defined(__DJUTIL_H_fixedpoint_LOADED))
#define __DJUTIL_H_fixedpoint_LOADED
namespace djutil {namespace fixedpoint {
    #include <iostream>

    #include <algorithm>
    #include <type_traits>

    //#include <cmath>
    #include <cfloat>
    #include <climits>
    #include <limits>

        template <class I, const I S, class F>
        I _float2fixed(const F& fltval)
        {
            if (isfinite(fltval)){return (I)(fltval * (F)(S));}
            else if (fltval == -INFINITY){return std::numeric_limits<I>::min();}
            else if (fltval == INFINITY){return std::numeric_limits<I>::max();}
            return (I)0;
        }

        template <class I, const I S, class F>
        F _fixed2float(const I& fixval)
        {
        	return fixval / (F)S;
        }
        template <class I, const I S>
        I _fixed_round_nearest_whole(const I& fixval)
        {
        	const I whole = S * (fixval / S);
        	return whole + (S * ((fixval - whole) / (S/2)));
        }
        template <class I, const I S>
        I _fixed_trunc(const I& fixval)
        {
        	return S * (fixval / S);
        }
        template <class I, const I S>
        I _fixed_floor(const I& fixval)
        {
        	return _fixed_trunc<I,S>(fixval) - (fixval % S ? S : I(0));
        }
        template <class I, const I S>
        I _fixed_ceil(const I& fixval)
        {
        	return _fixed_trunc<I,S>(fixval) + (fixval % S ? S : I(0));
        }

        template <class I, const I S>
        struct alignas(I) _fixedpointIS {
            using basetype = I;
            using this_type = _fixedpointIS<I,S>;
            basetype raw_value;
            inline basetype scaling_factor() const {return S;}

            this_type& operator=(const this_type& v){this->raw_value = v.raw_value; return *this;}

            explicit operator float() const {return _fixed2float<I,S,float>(this->raw_value);}
            explicit operator double() const {return _fixed2float<I,S,double>(this->raw_value);}
            explicit operator long double() const {return _fixed2float<I,S,long double>(this->raw_value);}

            friend std::ostream& operator<<(std::ostream& ostr, const this_type& self)
            {
            	ostr << float(self);
            	return ostr;
            }

            inline this_type& operator+=(const this_type& b) {this->raw_value += b.raw_value; return *this;}
            inline this_type& operator-=(const this_type& b) {this->raw_value -= b.raw_value; return *this;}
            inline this_type& operator*=(const this_type& b) {this->raw_value *= b.raw_value; this->raw_value /= S; return *this;}
            inline this_type& operator/=(const this_type& b) {return (*this) = (float(*this)/float(b));}

            inline this_type operator+(const this_type& b) const {return (this_type(*this) += b);}
            inline this_type operator-(const this_type& b) const {return (this_type(*this) -= b);}
            inline this_type operator*(const this_type& b) const {return (this_type(*this) *= b);}
            inline this_type operator/(const this_type& b) const {return (this_type(*this) /= b);}

            this_type round() const {
            	this_type rnd;
            	rnd.raw_value = _fixed_round_nearest_whole<I,S>(this->raw_value);
            	return rnd;
            }

            _fixedpointIS() {}
            _fixedpointIS(const this_type& other) : raw_value(other.raw_value) {}

            _fixedpointIS(const float& f) {this->raw_value = _float2fixed<I,S,float>(f);}
            _fixedpointIS(const double& f) {this->raw_value = _float2fixed<I,S,double>(f);}
            _fixedpointIS(const long double& f) {this->raw_value = _float2fixed<I,S,long double>(f);}

            template <typename I2=I>
            _fixedpointIS(const I2 iv) : raw_value(S*iv) {}
        };


}};
#endif

#if (defined(DJUTIL_NEEDS_linalg) && !defined(__DJUTIL_H_linalg_LOADED))
#define __DJUTIL_H_linalg_LOADED
namespace djutil {namespace linalg {
    #include <iostream>

    #include <algorithm>
    #include <type_traits>

    #include <cmath>
    #include <cfloat>
    #include <climits>
    #include <limits>
    #include <initializer_list>
    #include <unordered_map>

    const int _VECTOR_SWIZZLE_MAP[256] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,3,2,0,0,0,0,1,0,0,0,0,0,0,0,0,
       	0,0,0,0,1,0,1,3,0,1,2,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
       	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    const int _MATRIX_SWIZZLE_MAP[256] = {
    	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    	0,
    	 0, //a
    	 1, //b
    	 2, //c
    	 3, //d
    	 4, //e
    	 5, //f
    	 6, //g
    	 7, //h
    	 8, //i
    	 9, //j
    	 10, //k
    	 11, //l
    	 12, //m
    	 13, //n
    	 14, //o
    	 15, //p
    };
    template <typename F> inline F pi();
    template<> inline float pi<float>(){return 3.14159f;}
    template<> inline double pi<double>(){return 3.1415926536;}
    template<> inline long double pi<long double>(){return 3.1415926536L;}

    struct _vectorized_add {template <class T> static void call(T& a, const T& b){a += b;}};
    struct _vectorized_sub {template <class T> static void call(T& a, const T& b){a -= b;}};
    struct _vectorized_mul {template <class T> static void call(T& a, const T& b){a *= b;}};
    struct _vectorized_div {template <class T> static void call(T& a, const T& b){a /= b;}};

    template <class I, const I S> using _fixedpointIS = djutil::fixedpoint::_fixedpointIS<I,S>;

    template<class I, const I S, class FP=float>
    inline _fixedpointIS<I,S> sin(const _fixedpointIS<I,S>& x){return _fixedpointIS<I,S>(std::sin(FP(x)));}
    template <class T>
    inline T sin(const T& x){return std::sin(x);}

    template<class I, const I S, class FP=float>
    inline _fixedpointIS<I,S> cos(const _fixedpointIS<I,S>& x){return _fixedpointIS<I,S>(std::cos(FP(x)));}
    template <class T>
    inline T cos(const T& x){return std::cos(x);}

    template<class I, const I S, class FP=float>
    inline _fixedpointIS<I,S> tan(const _fixedpointIS<I,S>& x){return _fixedpointIS<I,S>(std::tan(FP(x)));}
    template <class T>
    inline T tan(const T& x){return std::tan(x);}

    template<class I, const I S, class FP=float>
    inline _fixedpointIS<I,S> asin(const _fixedpointIS<I,S>& x){return _fixedpointIS<I,S>(std::asin(FP(x)));}
    template <class T>
    inline T asin(const T& x){return std::asin(x);}

    template<class I, const I S, class FP=float>
    inline _fixedpointIS<I,S> acos(const _fixedpointIS<I,S>& x){return _fixedpointIS<I,S>(std::acos(FP(x)));}
    template <class T>
    inline T acos(const T& x){return std::acos(x);}

    template <const int L, class T>
    struct alignas(T) _basevec {
        T v[L];
        using scalar_type = T;
        inline size_t size() const {return L;}

        inline const T* cbegin() const {return this->v;}
        inline const T* cend() const {return this->v + L;}

        inline T* begin() {return this->v;}
        inline T* end() {return this->v + L;}

        friend std::ostream& operator<<(std::ostream& ostr, const _basevec<L,T>& vec)
        {
            ostr << '{';
            for (int i = 0, j = 1; i < L; i++, j++){
            	ostr << vec.v[i];
            	if (j < L){ostr << ", ";}
            }
            ostr << '}';
        	return ostr;
        }

        const T& operator[](const int i) const {return this->v[i];}
        T& operator[](const int i){return this->v[i];}

        template <const int N>
        _basevec<N-1,T> operator[](const char (&indices)[N]) const {
        	_basevec<N-1,T> sv;
        	for (int i = 0; i < (N-1); i++){sv[i] = this[0][_VECTOR_SWIZZLE_MAP[(unsigned char)(indices[i])]];}
        	return sv;
        }

        template <typename F>
        _basevec<L,T>& _vector_unary(const _basevec<L,T>& right)
        {
        	for (int i = 0; i < L; i++){F::template call<T>(this->v[i], right.v[i]);}
        	return *this;
        }

        template <typename F>
        _basevec<L,T>& _vector_unary(const T& right){
        	for (int i = 0; i < L; i++){F::template call<T>(this->v[i], right);}
        	return *this;
        }


        template <class RT> inline _basevec<L,T>& operator+=(const RT& other){return this->_vector_unary<_vectorized_add>(other);}
        template <class RT> inline _basevec<L,T>& operator-=(const RT& other){return this->_vector_unary<_vectorized_sub>(other);}
        template <class RT> inline _basevec<L,T>& operator*=(const RT& other){return this->_vector_unary<_vectorized_mul>(other);}
        template <class RT> inline _basevec<L,T>& operator/=(const RT& other){return this->_vector_unary<_vectorized_div>(other);}

        template <class RT> inline _basevec<L,T> operator+(const RT& other) const {return _basevec<L,T>(*this) += other;}
        template <class RT> inline _basevec<L,T> operator-(const RT& other) const {return _basevec<L,T>(*this) -= other;}
        template <class RT> inline _basevec<L,T> operator*(const RT& other) const {return _basevec<L,T>(*this) *= other;}
        template <class RT> inline _basevec<L,T> operator/(const RT& other) const {return _basevec<L,T>(*this) /= other;}

        inline _basevec<L,T> operator-() const {
        	_basevec<L,T> res = *this;
        	for (int i = 0; i < L; i++){res.v[i] = -(res.v[i]);}
        	return res;
        }

        inline _basevec<L,T> recip(const T n=T(1)) const {
            _basevec<L,T> res = *this;
            for (int i = 0; i < L; i++){res.v[i] = n/(res.v[i]);}
        	return res;
        }
        inline T hsum() const {
        	T s = T(0);
        	for (int i = 0; i < L; i++){s += (this->v[i]);}
        	return s;
        }
        template <const int L2=L, class T2=T>
        operator _basevec<L2,T2>() const {
        	_basevec<L2,T2> ret;
        	for (int i = 0; i < L && i < L2; i++){ret.v[i] = T2(this->v[i]);}
        	return ret;
        }
        _basevec() : v{} {}
        _basevec(const T _f) {
        	for (int i = 0; i < L; i++){this->v[i] = _f;}
        }
        /*
        template <class... Args>
        _basevec(const Args... args) : v{args...} {}
        */
        _basevec(const std::initializer_list<T>& il) : v{} {
        	std::copy_n(il.begin(), std::min(this->size(), il.size()), this->v);
        }

        _basevec(const _basevec<L-1, T>& _vec, const T _l) {
            //std::fill_n(this->v, L-1, (_l));
        	std::copy_n(_vec.v, (L-1), this->v);
        	this->v[L-1] = _l;
        }
        //template <class VT=T>


    };
    /*
    template <class LT, const int L, class T> inline _basevec<L,T> _basevec<L,T>::operator+(const LT l, const _basevec<L,T>& r) {return r+l;}
    template <class LT, const int L, class T> inline _basevec<L,T> _basevec<L,T>::operator-(const LT l, const _basevec<L,T>& r) {
        _basevec<L,T> res;
        for (int i = 0; i < L; i++){res.v[i] = l - r.v[i];}
        return res;
    }
    template <class LT, const int L, class T> inline _basevec<L,T> _basevec<L,T>::operator*(const LT l, const _basevec<L,T>& r) {return r*l;}

    template <class LT, const int L, class T> inline _basevec<L,T> _basevec<L,T>::operator/(const LT l, const _basevec<L,T>& r) {return r.recip() * l;}
    */

    using fvec2 = _basevec<2,float>;
    using fvec3 = _basevec<3,float>;
    using fvec4 = _basevec<4,float>;

    template <const int L, class T>
    T dot(const _basevec<L,T>& a, const _basevec<L,T>& b)
    {
    	return (a*b).hsum();
    }

    template <class T>
    inline _basevec<3,T> cross(const _basevec<3,T>& a, const _basevec<3,T>& b)
    {
    	return _basevec<3,T>{
    		a[1] * b[2] - b[1] * a[2],
    		a[2] * b[0] - b[2] * a[0],
    		a[0] * b[1] - b[0] * a[1]
    	};
    }

    template <const int L, class T, class FP=float>
    inline T length(const _basevec<L,T>& v){return std::sqrt(FP((v*v).hsum()));}

    template <const int L, class T>
    _basevec<L,T> normalize(const _basevec<L,T>& v)
    {
    	return v/length(v);
    }

    template <const int L, class T>
    inline _basevec<L,T> reflect(const _basevec<L,T>& v, const _basevec<L,T>& n)
    {
        return v - n*(dot(v,n)*T(2));
    }

    template <class T=float>
    inline T degrees(const T& rads){return ((T)180)*(rads/T(3.141593));}

    template <class T=float>
    inline T radians(const T& degs){return 3.141593*(degs/180.0);}

    template <const int L, class T>
    T angle_between(const _basevec<L,T>& a, const _basevec<L,T>& b)
    {
    	return acos(dot(a,b));
    }

    template <const int R, const int C, class T>
    struct alignas(T) _basemat {
        T m[R*C];
        using this_type = _basemat<R,C,T>;
        using transposed_type = _basemat<C,R,T>;
        using scalar_type = T;
        using rowvec_type = _basevec<C, T>;
        using colvec_type = _basevec<R, T>;

        inline size_t nrows() const {return R;}
        inline size_t ncols() const {return C;}
        inline size_t size() const {return R*C;}

        inline const T* operator[](const int r) const {
        	return this->m + (r * C);
        }
        inline T* operator[](const int r){
        	return this->m + (r * C);
        }

        inline rowvec_type getrow(const int r) const {return reinterpret_cast<const rowvec_type*>(this->m)[r];}
        inline void        setrow(const int r, const rowvec_type& v) {std::copy_n(v.v, C, this[0][r]);}

        colvec_type getcol(const int c) const {
        	colvec_type col;
        	for (int i = 0; i < R; i++){col[i] = this[0][i][c];}
        	return col;
        }
        void setcol(const int c, const colvec_type& v){
        	for (int i = 0; i < R; i++){this[0][i][c] = v[i];}
        }
        friend std::ostream& operator<<(std::ostream& ostr, const this_type& mat) {
            rowvec_type row;
            ostr << '[';
            for (int r = 0, r2 = 1; r < R; r++, r2++)
            {
            	std::copy_n(mat[r], mat.ncols(), row.v);
            	ostr << row;
            	if (r2 < R){ostr << ',' << ' ';}
            }
            ostr << ']';
            return ostr;
        }

        T operator[](const char ci) const {
        	return this->m[_MATRIX_SWIZZLE_MAP[(unsigned char)ci]];
        }
        template <const int R2, const int C2, class T2=T>
        _basemat<R2,C2,T> swizzle_rm(const char* indices) const {
        	_basemat<R2,C2,T2> sm;
        	for (int r = 0, i = 0; r < R2; r++)
        	for (int c = 0; c < C2; c++, i++){
        		sm[r][c] = T2(this->m[_MATRIX_SWIZZLE_MAP[(unsigned char)(indices[i])]]);
        	}
        	return sm;
        }

        //template <const int R2, class T2>
        colvec_type operator*(const rowvec_type& v) const {
            colvec_type res;
        	for (int i = 0; i < R; i++)
        	{
        	    res[i] = dot(this->getrow(i), v);
        	}
        	return res;
        }

        this_type operator*(const this_type& m) const {
            this_type res;
        	for (int r = 0; r < R; r++)
        	{
        		const rowvec_type a = this->getrow(r);
        		for (int c = 0; c < C; c++)
        		{
        		    const colvec_type b = m.getcol(c);
        			res[r][c] = dot(a, b);
        		}
        	}
        	return res;
        }

        this_type& operator*=(const this_type& m){
        	(*this) = (*this) * m;
        	return *this;
        }

        this_type& eye(const T v1=T(1), const T v0=T(0))
        {
        	return ((*this) = this_type(v1,v0));
        }

        template <const int R2, class T2>
        this_type& translate(const _basevec<R2,T2>& tv)
        {
        	for (int i = 0; i < R2 && i < R; i++)
        	{
        		this[0][i][C-1] += T(tv[i]);
        	}
        	return (*this);
        }

        template <const int R2, class T2>
        this_type translated(const _basevec<R2,T2>& tv) const {
        	return this_type(*this).translate(tv);
        }

        template <class T2>
        this_type& scale(const _basevec<R-1,T2>& s)
        {
            for (int d = 0; (d+1) < R && (d+1) < C; d++)
            {
            	this[0][d][d] *= s[d];
            }
            return *this;
        }

        template <class T2>
        this_type scaled(const _basevec<R-1, T2>& s){return this_type(*this).scale(s);}

        transposed_type transposed() const {
        	transposed_type t;
        	for (int r = 0; r < R; r++)
        	for  (int c = 0; c < C; c++)
            {
            	t[c][r] = this[0][r][c];
            }
        	return t;
        }

        template <const int R2=R, const int C2=C, class T2=T>
        void convert_into(_basemat<R2, C2, T2>& dst) const {
        	for (int row = 0; row < R && row < R2; row++)
        	for  (int col = 0; col < C && col < C2; col++){
        		dst[row][col] = T2(this[0][row][col]);
        	}
        }

        template <const int R2=R, const int C2=C, class T2=T>
        operator _basemat<R2,C2,T2>() const {
        	_basemat<R2,C2,T2> res(T2(1), T2(0));
        	this->convert_into(res);
        	return res;
        }

        _basemat() : m{} {}
        _basemat(const T v1, const T v0=T(0))
        {
        	for (int r = 0, c; r < R; r++)
        	{
        	    T* row = (*this)[r];
        		for (c = 0; c < C; c++)
        		{
        		    row[c] = (c == r ? v1 : v0);
        		}
        	}
        }
        _basemat(const std::initializer_list<rowvec_type>& vil) : m{}
        {
        	for (int row = 0; row < R && row < vil.size(); row++)
        	{
        	    std::copy_n((*(vil.begin()+row)).v, C, this[0][row]);
        	}
        }

    };

    using fmat2 = _basemat<2,2,float>;
    using fmat3 = _basemat<3,3,float>;
    using fmat4 = _basemat<4,4,float>;

    template <class FP=float>
    _basemat<3,3,FP> rotationCCW(const _basevec<3,FP>& _axis, const FP _angle, const bool _radians=true)
    {
    	const FP angle = (_radians ? _angle : radians(_angle));
    	const FP s = sin(angle), c = cos(angle), u = FP(1) - c;
    	const _basevec<3,FP> axis = normalize(_axis);
    	const FP ax = axis[0], ay = axis[1], az = axis[2];
    	return _basemat<3,3,FP>{
    	    {u*ax*ax + c, u*ax*ay - s*az, u*ax*az + s*ay},
    	    {u*ax*ay + s*az, u*ay*ay + c, u*ay*az - s*ax},
    	    {u*ax*az - s*ay, u*ay*az + s*ax, u*az*az + c}
    	};
    }
    bool _inverse4x4_invertible_dummy = false;
    template <class cv3_scalar=float>
    _basemat<4,4,cv3_scalar> inverse(const _basemat<4,4,cv3_scalar>& mat, bool& invertible=_inverse4x4_invertible_dummy)
    {
        //Adapted from Mesa's implementation of GLU.
        _basemat<4,4,cv3_scalar> invmat;
        cv3_scalar *inv = invmat.m, det;
        const cv3_scalar* m = mat.m;
        int i;
        inv[0] = m[5]  * m[10] * m[15] -
                 m[5]  * m[11] * m[14] -
                 m[9]  * m[6]  * m[15] +
                 m[9]  * m[7]  * m[14] +
                 m[13] * m[6]  * m[11] -
                 m[13] * m[7]  * m[10];

        inv[4] = -m[4]  * m[10] * m[15] +
                  m[4]  * m[11] * m[14] +
                  m[8]  * m[6]  * m[15] -
                  m[8]  * m[7]  * m[14] -
                  m[12] * m[6]  * m[11] +
                  m[12] * m[7]  * m[10];

        inv[8] = m[4]  * m[9] * m[15] -
                 m[4]  * m[11] * m[13] -
                 m[8]  * m[5] * m[15] +
                 m[8]  * m[7] * m[13] +
                 m[12] * m[5] * m[11] -
                 m[12] * m[7] * m[9];

        inv[12] = -m[4]  * m[9] * m[14] +
                   m[4]  * m[10] * m[13] +
                   m[8]  * m[5] * m[14] -
                   m[8]  * m[6] * m[13] -
                   m[12] * m[5] * m[10] +
                   m[12] * m[6] * m[9];

        inv[1] = -m[1]  * m[10] * m[15] +
                  m[1]  * m[11] * m[14] +
                  m[9]  * m[2] * m[15] -
                  m[9]  * m[3] * m[14] -
                  m[13] * m[2] * m[11] +
                  m[13] * m[3] * m[10];

        inv[5] = m[0]  * m[10] * m[15] -
                 m[0]  * m[11] * m[14] -
                 m[8]  * m[2] * m[15] +
                 m[8]  * m[3] * m[14] +
                 m[12] * m[2] * m[11] -
                 m[12] * m[3] * m[10];

        inv[9] = -m[0]  * m[9] * m[15] +
                  m[0]  * m[11] * m[13] +
                  m[8]  * m[1] * m[15] -
                  m[8]  * m[3] * m[13] -
                  m[12] * m[1] * m[11] +
                  m[12] * m[3] * m[9];

        inv[13] = m[0]  * m[9] * m[14] -
                  m[0]  * m[10] * m[13] -
                  m[8]  * m[1] * m[14] +
                  m[8]  * m[2] * m[13] +
                  m[12] * m[1] * m[10] -
                  m[12] * m[2] * m[9];

        inv[2] = m[1]  * m[6] * m[15] -
                 m[1]  * m[7] * m[14] -
                 m[5]  * m[2] * m[15] +
                 m[5]  * m[3] * m[14] +
                 m[13] * m[2] * m[7] -
                 m[13] * m[3] * m[6];

        inv[6] = -m[0]  * m[6] * m[15] +
                  m[0]  * m[7] * m[14] +
                  m[4]  * m[2] * m[15] -
                  m[4]  * m[3] * m[14] -
                  m[12] * m[2] * m[7] +
                  m[12] * m[3] * m[6];

        inv[10] = m[0]  * m[5] * m[15] -
                  m[0]  * m[7] * m[13] -
                  m[4]  * m[1] * m[15] +
                  m[4]  * m[3] * m[13] +
                  m[12] * m[1] * m[7] -
                  m[12] * m[3] * m[5];

        inv[14] = -m[0]  * m[5] * m[14] +
                   m[0]  * m[6] * m[13] +
                   m[4]  * m[1] * m[14] -
                   m[4]  * m[2] * m[13] -
                   m[12] * m[1] * m[6] +
                   m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11] +
                  m[1] * m[7] * m[10] +
                  m[5] * m[2] * m[11] -
                  m[5] * m[3] * m[10] -
                  m[9] * m[2] * m[7] +
                  m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11] -
                 m[0] * m[7] * m[10] -
                 m[4] * m[2] * m[11] +
                 m[4] * m[3] * m[10] +
                 m[8] * m[2] * m[7] -
                 m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] +
                   m[0] * m[7] * m[9] +
                   m[4] * m[1] * m[11] -
                   m[4] * m[3] * m[9] -
                   m[8] * m[1] * m[7] +
                   m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10] -
                  m[0] * m[6] * m[9] -
                  m[4] * m[1] * m[10] +
                  m[4] * m[2] * m[9] +
                  m[8] * m[1] * m[6] -
                  m[8] * m[2] * m[5];

        det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        if (det == 0)
        {
    		//if (invertible != NULL){*invertible = false;}
    		invertible = false;
            return invmat;
    	}

        //det = 1.0 / det;
        det = cv3_scalar(1) / det;
        //for (i = 0; i < 16; i++)
        //    out->storage[i] = inv[i] * det;
        /*
        #define cv3u4(i) invmat.rows[i] = cv3_vms_inl(invmat.rows[i], det);
        _cv3_unroll4_macro
        #undef cv3u4
        if (invertible != NULL){*invertible = true;}
        */
        for (i = 0; i < 16; i++){
        	invmat.m[i] *= det;
        }
        invertible = true;
        return invmat;
    }


}};
#endif
#if (defined(DJUTIL_NEEDS_ezstr) && !defined(__DJUTIL_H_ezstr_LOADED))
#define __DJUTIL_H_ezstr_LOADED
#include <codecvt>
#include <locale>
namespace djutil {namespace ezstr {
    //using _bstr_t = std::basic_string;

    //bool isutf(const char& c) {return (((c)&0xC0)!=0x80);}

    bool djstr2_isu8start(const char& chr){return (((chr)&0xC0)!=0x80);}

    static const uint_least32_t djstr2_offsetsFromUTF8[6] = {
        0x00000000UL, 0x00003080UL, 0x000E2080UL,
        0x03C82080UL, 0xFA082080UL, 0x82082080UL
    };

    static const char djstr2_trailingBytesForUTF8[256] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
    };

    /* returns length of next utf-8 sequence */
    int djstr2_u8_seqlen(char *s)
    {
        return djstr2_trailingBytesForUTF8[(unsigned int)(unsigned char)s[0]] + 1;
    }

    int u8c_to_u32c(char32_t& dst, char* src)
    {
        //if (dst == NULL){return -1;}
        if (src == NULL){return -2;}
        char32_t ch;
        int nb;

        nb = djstr2_trailingBytesForUTF8[(unsigned char)*src];
        if (nb > 3){return 0;}
        ch = 0;
        switch (nb) {
            /* these fall through deliberately */
            case 3: ch += (unsigned char)*src++; ch <<= 6;
            case 2: ch += (unsigned char)*src++; ch <<= 6;
            case 1: ch += (unsigned char)*src++; ch <<= 6;
            case 0: ch += (unsigned char)*src++;
        }
        ch -= djstr2_offsetsFromUTF8[nb];
        dst = ch;
        return nb+1;
    }

    int u8getc(char32_t& dst, std::istream& src) {
        char inu8[4] = {};
        inu8[0] = src.get();
        if (src.eof()){return 0;}
        size_t ntb = djstr2_trailingBytesForUTF8[(unsigned char)(inu8[0])];
        if (ntb > 3){return 0;}
        size_t nr = src.read(inu8+1, ntb).gcount();
        if (nr < ntb){return 0;}
        return u8c_to_u32c(dst, inu8);
    }



    template <class MB, class WC, const int MBCW=4, class S=std::mbstate_t>
    int wide2mb(std::basic_string<MB>& out, const std::basic_string<WC>& in)
    {
		std::vector<MB> app = {};
        using cvt_t = std::codecvt<WC,MB,S>;
        std::locale mylocale;
        const cvt_t& facet = std::use_facet<cvt_t>(mylocale);

    	int count = 0;
    	if (in.size() == 0){out.clear(); return 0;}
    	const WC* wcp = &(in.front());
    	const WC* ine = &(in.back());
    	while (wcp <= ine)
    	//for (const WC& wc : in)
    	{
			const WC& wc = *wcp;
    	    auto state = S();
    		const WC* from_next;
    		MB* to_next;
    		MB mbchr[MBCW+1] = {};
    		std::codecvt_base::result result = facet.out(state, &wc, (&wc)+1, from_next, mbchr, (mbchr)+MBCW+1, to_next);
    	    //if (from_next > ine){break;}
    	    wcp = from_next;
    	    if (to_next > mbchr)
    	    //if (result != std::codecvt_base::result::error)
    	    {
				for (MB* c = mbchr; c < to_next; c++){app.push_back(*c);}
    	        count++;
    	    }
    	}
    	out.append(app.data(), app.size());
    	return count;
    }

    template <class WC, class MB=char, class S=std::mbstate_t>
    int mb2wide(std::basic_string<WC>& out, const std::basic_string<MB>& in)
    {
        using cvt_t = std::codecvt<WC,MB,S>;
        std::locale mylocale;
        const cvt_t& facet = std::use_facet<cvt_t>(mylocale);

    	int count = 0;
    	const MB *mb_start = &in.front(), *mb_end = &in.back();
    	const MB* from_next = mb_start;
    	WC* to_next;

    	while (from_next < mb_end)
    	{
    	    auto state = S();
    		WC wchr[1] = {};
    		std::codecvt_base::result result = facet.in(state,
    		         from_next, mb_end, from_next,
    		         wchr, wchr+1, to_next
    		);
    		if (to_next > wchr){out.push_back(*wchr); count++;}
    	}
    	return count;
    }




    std::basic_string<char32_t> ascii_to_utf32(const std::string& ascii)
    {
        if (ascii == ""){return U"";}
    	std::basic_string<char32_t> u32; u32.resize(ascii.size());
    	std::copy_n((const unsigned char*)&ascii.front(), ascii.size(), &u32.front());
    	return u32;
    }
    std::string utf32_to_utf8(const std::basic_string<char32_t>& utf32)
    {
		/*
    	std::string utf8 = ""; wide2mb(utf8, utf32);
    	return utf8;
    	*/
    	return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.to_bytes(utf32);
    }
    std::basic_string<char16_t> utf8_to_utf16(const std::string& utf8) {
		return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(utf8);
	}
    std::basic_string<char32_t> utf8_to_utf32(const std::string& utf8) {
		return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(utf8);
	}

	std::string utf16_to_utf8(const std::basic_string<char16_t>& utf16) {
		return std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t>{}.to_bytes(utf16);
	}



    template <class CT>
    bool partition(std::basic_string<CT>& head, std::basic_string<CT>& tail, const std::basic_string<CT>& str, const std::basic_string<CT> sep, const size_t startpos=0)
    {
        if (startpos >= str.size())
        {
            head = std::basic_string<CT>();
            tail = std::basic_string<CT>();
            return false;
        }
    	const size_t findloc = str.find(sep, startpos);
    	if (findloc == std::basic_string<CT>::npos)
    	{
    		head = str.substr(startpos);
    		tail = std::basic_string<CT>();
    		return false;
    	}
    	else
    	{
    	    const size_t termend = findloc + sep.size();
    		head = str.substr(startpos, findloc - startpos);
    		tail = (termend < str.size() ? str.substr(termend) : std::basic_string<CT>());
    		return true;
    	}
    }

    template <class CT, class SLT=std::vector<std::basic_string<CT>>>
    void split(SLT& splitlist, const std::basic_string<CT>& str, const std::basic_string<CT> sep, const size_t startpos=0)
    {
        const size_t _npos = std::basic_string<CT>::npos;
        const size_t _sepsz = sep.size();
        if (_sepsz == 0 || str.size() == 0){return;}
    	//std::basic_string<CT> head, tail = str.substr(startpos);
    	size_t curpos = startpos;
    	while (true)
    	{
    	    //bool res = partition<CT>(head, tail, tail, sep);
    		//if (res && (head.size() == 0)){continue;}
    		//splitlist.push_back(head);
    		//if (!res){break;}
    		size_t newpos = str.find(sep, curpos);
    		auto sub = str.substr(curpos, (newpos != _npos ? newpos-curpos : _npos));
    	    splitlist.push_back(sub);
    		if (newpos == _npos){break;}
    		curpos = newpos + _sepsz;
    	}
    }
    template <class CT, class SLT=std::vector<std::basic_string<CT>>>
    SLT split(const std::basic_string<CT>& str, const std::basic_string<CT> sep, const size_t startpos=0)
    {
    	SLT splitlist; split(splitlist, str, sep, startpos);
    	return splitlist;
    }

    template <class CT, class SLT=std::vector<std::basic_string<CT>>>
    void splitlines(SLT& splitlist, const std::basic_string<CT>& str){
        const size_t _npos = -1;
        const CT
            cr = CT('\r'),
            lf = CT('\n')
        ;
        auto curitr = str.cbegin();
        auto strend = str.cend();
        while (curitr < strend){
            std::basic_string<CT> ln;
            auto oldpos = curitr;
            auto crfind = std::find(curitr, strend, cr);
            auto lffind = std::find(curitr, strend, lf);

            if (crfind == strend && lffind == strend){
                std::copy(curitr, strend, std::back_inserter(ln));
                curitr = strend;
            }
            else if (lffind < crfind){
                std::copy(curitr, lffind, std::back_inserter(ln));
                curitr = lffind+1;
            }
            else if (crfind < lffind){
                std::copy(curitr, crfind, std::back_inserter(ln));
                curitr = crfind+1;
                if (lffind != strend && (crfind+1) == lffind){
                    curitr++;
                }
            }
            if (ln.size() > 0){splitlist.push_back(ln);}
        }
    }
    template <class CT>
    bool startswith(const std::basic_string<CT>& str, const std::basic_string<CT>& term)
    {
    	if (term.size() > str.size()){return false;}
    	for (size_t i = 0; i < term.size(); i++)
    	{
    		if (term[i] != str[i]){return false;}
    	}
    	return true;
    }
    template <class CT>
    bool startswith(const std::basic_string<CT>& str, const CT* term)
    {
    	return startswith(str, std::basic_string<CT>(term));
    }

    template <class CT>
    bool endswith(const std::basic_string<CT>& str, const std::basic_string<CT>& term)
    {
    	if (term.size() > str.size()){return false;}
    	for (size_t i = 0, j = (str.size()-term.size()); j < str.size(); i++, j++)
    	{
    		if (term[i] != str[j]){return false;}
    	}
    	return true;
    }
    template <class CT>
    bool endswith(const std::basic_string<CT>& str, const CT* term)
    {
    	return endswith(str, std::basic_string<CT>(term));
    }

    template <class CT, class IT>
    std::basic_string<CT> itrjoin(const std::basic_string<CT>& sep, const IT istart, const IT iend){
        std::basic_string<CT> res;
        auto it = istart;
        while (it != iend)
    	{
    		res.append(*it);
    		it++;
    		if (it != iend){res.append(sep);}
    		else {break;}
    	}
    	return res;
    }

    template <class CT, class LT=std::vector<std::basic_string<CT>>>
    std::basic_string<CT> join(const LT& subs, const std::basic_string<CT> sep=std::basic_string<CT>())
    {
    	return itrjoin(sep, subs.cbegin(), subs.cend());
    }

    template <class CT, class LT=std::vector<std::basic_string<CT>>>
    std::basic_string<CT> join(const LT& subs, const CT* sep)
    {
    	return join(subs, std::basic_string<CT>(sep));
    }

    template <class CT>
    bool rpartition(std::basic_string<CT>& head, std::basic_string<CT>& tail, const std::basic_string<CT>& str, const std::basic_string<CT>& sep) {
    	std::vector<std::basic_string<CT>> splits = {};
    	split<CT>(splits, str, sep);
    	if (splits.size() == 0){return false;}
    	tail = splits.back(); splits.pop_back();
    	head = join<CT>(splits, sep);
    	return true;
    }


    const std::u32string WHITESPACE = U" \n\r\t";
    const std::string WHITESPACE_STR = " \n\r\t";
    bool u8skipws(std::istream& src, const std::u32string& ws = WHITESPACE) {
        while (src) {
            char32_t c;
            auto lastpos = src.tellg();
            if (u8getc(c, src) == 0){return true;}
            //else if (c == U' ' || c == U'\n' || c == U'\r' || c == U'\t'){continue;}
            bool isws = false;
            for (auto dc : ws){
                if (c == dc){isws = true; break;}
            }
            if (isws){continue;}
            src.seekg(lastpos);
            break;
        }
        return false;
    }
    bool u8skip_until(char32_t& termchr, std::istream& src, const std::u32string& terminators) {
        while (src) {
            char32_t c;
            if (u8getc(c, src) == 0){break;}
            bool isterm = false;
            for (auto tc : terminators) {
                if (c == tc){termchr = c; isterm = true; break;}
            }
            if (isterm){return true;}
        }
        return false;
    }
    size_t u8reads(std::u32string& dst, std::istream& src, const std::u32string& delims = WHITESPACE){
        auto oldpos = src.tellg();
        dst.clear();
        if (!u8skipws(src, delims)){
            while (src){
                char32_t c;
                if (u8getc(c, src) == 0){break;}
                bool hitdelim = false;
                for (auto dc : delims){
                    if (c == dc){hitdelim = true; break;}
                }
                if (hitdelim){break;}
                dst.push_back(c);
            }
        }
        return size_t(src.tellg()) - size_t(oldpos);
    }
    size_t u8reads(std::string& dst, std::istream& src, const std::string& delims = WHITESPACE_STR){
        auto oldpos = src.tellg();
        dst.clear();
        const std::u32string delims_u32 = utf8_to_utf32(delims);
        std::u32string dst32;
        size_t res = u8reads(dst32, src, delims_u32);
        dst = utf32_to_utf8(dst32);
        return res;
    }

    size_t parse_strlit(std::u32string& dst, std::istream& src) {
        char32_t quotechr;
        auto oldpos = src.tellg();
        if (u8getc(quotechr, src) == 0){
            throw std::runtime_error("Premature EOF in string literal!");
        }
        else if (quotechr != U'"'){
            throw std::runtime_error("Starting character is not a double-quote!");
        }
        bool loop = true;
        while (true) {
            char32_t c, c2;
            if (u8getc(c, src) == 0){throw std::runtime_error("Bad character in string literal!");}

            switch (c) {
                case U'"': {loop = false; break;}
                case U'\\': {
                    if (u8getc(c2, src) == 0){throw std::runtime_error("Bad character in escape sequence!");}
                    switch (c2) {
                        case U'\\':
                        case U'"':
                            dst.push_back(c2);
                            break;
                        case U'r': dst.push_back(U'\r'); break;
                        case U'n': dst.push_back(U'\n'); break;
                        case U't': dst.push_back(U'\t'); break;
                        default: ;
                    }
                    break;
                }
                default: dst.push_back(c);
            }
            if (!loop){break;}
        }
        return size_t(src.tellg()) - size_t(oldpos);
    }

    size_t parse_strlit(std::string& dst, std::istream& src) {
        std::u32string dst32;
        size_t nb = parse_strlit(dst32, src);
        dst = utf32_to_utf8(dst32);
        return nb;
    }

}};
#endif

#if (defined(DJUTIL_NEEDS_platform) && !defined(__DJUTIL_H_platform_LOADED))
#define __DJUTIL_H_platform_LOADED
namespace djutil{namespace platform{
	enum struct ostype : int {
		unknown = 0,
		win32 = 1,
		gnu_linux = 2,
		android = 3
	};

	#if (defined(_WIN32) || defined(_WIN64))
	    #define DJUTIL_OSTYPE 1
	#elif (defined(__linux__) || defined(ANDROID) || defined(__ANDROID__))
	    #if (defined(ANDROID) || defined(__ANDROID__))
	        #define DJUTIL_OSTYPE 3
	    #else
	        #define DJUTIL_OSTYPE 2
	    #endif
	    #define DJUTIL_POSIX

	#else
	    #define DJUTIL_OSTYPE 0
	#endif

	static const ostype OSTYPE = ostype(DJUTIL_OSTYPE);
	static const bool IS_POSIX =
	#if (defined(DJUTIL_POSIX))
	    true
	#else
	    false
	#endif
	;

	inline std::string osname(const ostype _ost=OSTYPE) {
		switch (_ost) {
			case ostype::win32: return "Microsoft Windows";
			case ostype::gnu_linux: return "GNU/Linux";
			case ostype::android: return "Android";
			default: return "<unknown OS>";
		}
	}
}};
#endif

#if (defined(DJUTIL_NEEDS_pathman) && !defined(__DJUTIL_H_pathman_LOADED))
#define __DJUTIL_H_pathman_LOADED
namespace djutil {namespace pathman {
    #include <sstream>
    #include <cstdio>
    enum struct pathsyntax : char {
    	posix = '/',
    	win32 = '\\',
    	macos = ':'
    };
    static const std::string _WIN32_DRIVELETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    static const pathsyntax _DEFAULT_SYNTAX =
    #if (defined(DJUTIL_POSIX))
        pathsyntax::posix
    #elif (DJUTIL_OSTYPE == 1)
        pathsyntax::win32
    #endif
    ;

    #if (defined(DJUTIL_POSIX))
     #include <unistd.h>
     #include <dirent.h>
     #include <sys/stat.h>
     #include <sys/types.h>
     #include <sys/param.h>
     char _PATH_TMP1[4097], _PATH_TMP2[4097];
    #elif (DJUTIL_OSTYPE == 1)
     #include <windows.h>
    #endif

    template <class C> using basestr = std::basic_string<C>;
    template <class C>
    bool is_path_valid(const pathsyntax ps, const basestr<C>& path){
    	switch (ps){
    		case pathsyntax::posix: {
    			if (path.size() == 0 || path.front() != C('/')){return false;}
    			return true;
      		}
      		case pathsyntax::win32: {
      			if (path.size() < 3){return false;}
      			bool vdl = false;
      			for (const auto l : _WIN32_DRIVELETTERS) {
      				if (path.front() == C(l)){
      					vdl = true;
      					break;
      				}
      			}

      			if (!vdl || path[1] != C(':') || path[2] != C('\\')){return false;}
      			return true;
      		}
      		default: return false;
    	}
    }

    std::string get_cwd() {
        #if (defined(DJUTIL_POSIX))
         memset(_PATH_TMP1, 0, sizeof(_PATH_TMP1));
         getcwd(_PATH_TMP1, sizeof(_PATH_TMP1)-1);
         return std::string(_PATH_TMP1) + std::string("/");
        #elif (DJUTIL_OSTYPE == 1)
         std::u16string ws;
         size_t blen = GetCurrentDirectoryW(0, nullptr);
         ws.resize(blen);
         GetCurrentDirectoryW(blen, (wchar_t*)&ws.front());
         ws.back() = '\\';
         return ezstr::utf16_to_utf8(ws);
        #else
         return "";
        #endif
    }

    template <class C>
    void split(basestr<C>& head, basestr<C>& tail, const basestr<C>& path, const pathsyntax ps=_DEFAULT_SYNTAX) {
    	const C sep = C(ps);
    	const basestr<C> seps(&sep, 1);
    	head.clear(); tail.clear();
    	if (path.size() == 0){return;}
    	switch (ps) {
    	    case pathsyntax::win32: {
    	        bool endsep = (path.size() > 2 && path.back() == sep);
    	        basestr<C> p = path;
    	        if (endsep){p.pop_back();}
    	        ezstr::rpartition(head, tail, p, seps);
    	        if (endsep){tail.push_back(sep);}
    	        if (!endsep){head.push_back(sep);}
    	    	break;
    	    }
    		case pathsyntax::posix: {
    		    if (path == seps){
    		    	head = seps;
    		    	return;
    		    }
    			bool endsep = (path.size() > 1 && path.back() == sep);
    			basestr<C> p = path;
    		    if (endsep){p.pop_back();}
    		    ezstr::rpartition(head, tail, p, seps);
    		    if (endsep){tail.push_back(sep);}
    		    if ((head.size() == 0) || !endsep){head.push_back(sep);}
    		    break;
    		}
    		default: break;
    	}

    }

    template <class C>
    void expand_relposix(basestr<C>& out, const basestr<C>& rootdir, const basestr<C>& rpp, const pathsyntax ps=_DEFAULT_SYNTAX) {
    	const C sep = C(ps);
    	const basestr<C> seps(&sep, 1);
    	out.clear();
    	//std::vector<basestr<C>> rpptoks = {};
    	switch (ps) {
    		case pathsyntax::posix: {
    		    out = rootdir;
    		    if (rootdir.back() == sep){out.pop_back();}
    		    out += rpp;
    			break;
    		}
    		case pathsyntax::win32: {
    		    out = rootdir;
    		    if (rootdir.back() == sep){out.pop_back();}
    		    const C psep = C(pathsyntax::posix);
    		    const basestr<C> pseps(&psep, 1);
    		    auto rpptoks = ezstr::split<C>(rpp, pseps);

    		    auto joined = ezstr::join<C>(rpptoks, seps);
    		    out += joined;

    			break;
    		}
    		default: break;
    	}
    }

    bool is_dir(const std::string& p) {
        #if (defined(DJUTIL_POSIX))
         struct stat _pinfo;
         if (lstat(p.c_str(), &_pinfo) < 0){return false;}
         return S_ISDIR(_pinfo.st_mode);
        #elif (DJUTIL_OSTYPE == 1)
         const std::basic_string<char16_t> lp = ezstr::utf8_to_utf16(p);
         DWORD fa = GetFileAttributesW((LPCWSTR)(lp.c_str()));
         return bool(fa & FILE_ATTRIBUTE_DIRECTORY);
        #endif
        return false;
    }
    bool is_file(const std::string& p) {
        #if (defined(DJUTIL_POSIX))
         struct stat _pinfo;
         if (lstat(p.c_str(), &_pinfo) < 0){return false;}
         return S_ISREG(_pinfo.st_mode);
        #elif (DJUTIL_OSTYPE == 1)
         const std::basic_string<char16_t> lp = ezstr::utf8_to_utf16(p);
         DWORD fa = GetFileAttributesW((LPCWSTR)(lp.c_str()));
         return !bool(fa & FILE_ATTRIBUTE_DIRECTORY);
        #endif
        return false;
    }

    template <class ELT>
    bool listdir(ELT& out_list, const std::string& in_dirname) {
        #if (defined(DJUTIL_POSIX))
         DIR* dp;
         struct dirent* ent;
         if ((dp = opendir(in_dirname.c_str())) == nullptr){
             return false;
         }
         while ((ent = readdir(dp))) {
             const std::string dname = ent->d_name;
             out_list.push_back(dname);
         }
         closedir(dp);
         return true;
        #elif (DJUTIL_OSTYPE == 1)

         WIN32_FIND_DATAW fdw;
         std::string idn = in_dirname;
         if (idn.size() == 0){idn = get_cwd();}
         if (idn.back() != '/' && idn.back() != '\\'){idn.push_back('\\');}
         idn.push_back('*');
         const std::basic_string<char16_t> wdn = ezstr::utf8_to_utf16(idn);
         std::cout << "listdir: in_dirname = " << idn << "\n";
         HANDLE hnd;
         if ((hnd = FindFirstFileW((LPCWSTR)(wdn.c_str()), &fdw)) == INVALID_HANDLE_VALUE){std::cout << "WARNING: FindFirstFileW returned INVALID_HANDLE_VALUE!\n"; return false;}
         do {
             const std::basic_string<char16_t> dnamew = (const char16_t*)(fdw.cFileName);
             out_list.push_back(ezstr::utf16_to_utf8(dnamew));
             //std::cout << "out_list.back() = " << out_list.back() << "\n";
         }
         while (FindNextFileW(hnd, &fdw) != 0);
         FindClose(hnd);

         /*
         WIN32_FIND_DATAA fda;
         HANDLE hnd;
         if ((hnd = FindFirstFileA(in_dirname.c_str(), &fda)) == INVALID_HANDLE_VALUE){std::cout << "WARNING: FindFirstFileW returned INVALID_HANDLE_VALUE!\n"; return false;}
         while (FindNextFileA(hnd, &fda) != 0 || GetLastError() != ERROR_NO_MORE_FILES){
             out_list.push_back(fda.cFileName);
             std::cout << "out_list.back() = " << out_list.back() << "\n";
         }
         FindClose(hnd);
         */
         return true;
        #endif
        return false;
    }

}};
#endif

#if (defined(DJUTIL_NEEDS_ufpio) && !defined(__DJUTIL_H_ufpio_LOADED))
#define __DJUTIL_H_ufpio_LOADED
#include <cstring>
#include <cstdio>

#if (DJUTIL_OSTYPE == 1)
 /*
 #define _DJUTIL_UFPIO_WIN32
 #include <windows.h>
 #ifdef __GLIBCXX__
  #include <ext/stdio_filebuf>
 #endif
 */
#endif
namespace djutil {namespace ufpio {
	namespace ezstr = djutil::ezstr;
	namespace platform = djutil::platform;
	using u32str = std::u32string;

	FILE* u8fopen(const std::string& path, const std::string& mode) {
		FILE* fp = nullptr;
		#if (defined(_DJUTIL_UFPIO_WIN32))
		 std::wstring wp = ezstr::utf8_to_utf16(path);
		 std::wstring wm = ezstr::utf8_to_utf16(mode);
		 fp = _wfopen(wp.c_str(), wm.c_str());
		#else
		 fp = fopen(path.c_str(), mode.c_str());
		#endif
		return fp;
	}
	FILE* ufopen(const u32str& path, const std::string& mode) {
		return u8fopen(ezstr::utf32_to_utf8(path), mode);
	}
	template <class charT, class traits = std::char_traits<charT>>
	#if (defined(_DJUTIL_UFPIO_WIN32) && defined(__GLICBXX__))
	 #define _DJUTIL_UFPIO_USING_STDIO
	 using basic_ufilebuf = __gnu_cxx::stdio_filebuf<charT, traits>;
	#else
	 #define _DJUTIL_UFPIO_USING_FILEBUF
	 using basic_ufilebuf = std::basic_filebuf<charT, traits>;
	#endif
	template <class charT, class traits = std::char_traits<charT>>
	class basic_ufstream : public std::basic_iostream<charT, traits> {
		private:
		    basic_ufilebuf<charT, traits>* _ufb = nullptr;
		    FILE* _stdio_hnd = nullptr;
		public:
		    bool is_open() const {return this->_ufb != nullptr;}
		    void close() {
				if (this->is_open()){
					this->rdbuf(nullptr);

					delete this->_ufb;
					this->_ufb = nullptr;

					if (this->_stdio_hnd != nullptr){fclose(this->_stdio_hnd); this->_stdio_hnd = nullptr;}
				}
			}
			bool open(const std::string& u8path, const std::ios_base::openmode mode) {
				this->close();
				bool _ok = true;
				#if (defined(_DJUTIL_UFPIO_USING_STDIO))
				    std::string _cmode = "";
				    const bool _rflag = (mode & std::ios_base::in);
				    const bool _wflag = (mode & std::ios_base::out);
				    const bool _bflag = (mode & std::ios_base::binary);
				    const bool _ateflag = (mode & std::ios_base::ate);
				    if (_rflag) {
						_cmode = "r";
						if (_bflag) {_cmode += "b";}
						if (_wflag) {_cmode += "+";}
					}
				    else if (_wflag) {
						_cmode = "w";
						if (_bflag) {_cmode += "b";}
				    }
				    this->_stdio_hnd = u8fopen(u8path, _cmode);
				    if (this->_stdio_hnd == nullptr){_ok = false;}
				    else {
						if (_ateflag){fseek(this->_stdio_hnd, 0, SEEK_END);}
						this->_ufb = new basic_ufilebuf<charT,traits>(this->_stdio_hnd, mode);
					}
				#else
				    this->_ufb = new basic_ufilebuf<charT,traits>();
				    this->_ufb->open(u8path, mode);
				    if (!this->_ufb->is_open()){_ok = false;}
				#endif
				if (!_ok){this->close();}
				else {this->rdbuf(this->_ufb);}
				return _ok;
			}
			bool open(const u32str& path, const std::ios_base::openmode mode) {
				return this->open(djutil::ezstr::utf32_to_utf8(path), mode);
			}
			basic_ufstream() : std::basic_iostream<charT,traits>(nullptr) {}
			basic_ufstream(const std::string& u8path, const std::ios_base::openmode mode) : std::basic_iostream<charT,traits>(nullptr) {this->open(u8path, mode);}
			basic_ufstream(const u32str& u32path, const std::ios_base::openmode mode) : std::basic_iostream<charT,traits>(nullptr) {this->open(u32path, mode);}
			~basic_ufstream() {
				this->close();
			}
	};
	using ufstream = basic_ufstream<char>;
}};
 #ifdef _DJUTIL_UFPIO_WIN32
  #undef _DJUTIL_UFPIO_WIN32
 #endif
#endif

#if (defined(DJUTIL_NEEDS_vircon) && !defined(__DJUTIL_H_vircon_LOADED))
#define __DJUTIL_H_vircon_LOADED
namespace djutil {namespace vircon {
    #include <iostream>
    #include <sstream>
    #include <vector>
    #include <functional>

	using u32string = std::basic_string<char32_t>;
	using u32strio = std::basic_stringstream<char32_t>;

	using u32strlist = std::vector<u32string>;


	enum class vcon_inputclass : int {
		null = 0,
	    character = 1,
	    enter = 2,
	    tab = 3,
	    backspace = 4,
	    del = 5,
	    ins = 6,
	    recall = 7,
	    curmove = 8
	};


	struct ConsoleInputEvent {
		vcon_inputclass type = vcon_inputclass::null;
		char32_t inchr = U'\0';
		int recall_shift = 0; // 1 means down, -1 means up.
		int curmove_shift = 0; // 1 means next, -1 means previous.
	};

	using _inevtlist_t = std::vector<ConsoleInputEvent>;

	template <class UG, class UL> struct VirtualConsole;

    template <class UG, class UL>
	struct ccmdargs_t {
		VirtualConsole<UG,UL>& vcon;
		const std::u32string& args;
		UG& userglobal;
		UL& userlocal;
	};

	template <class UG, class UL>
	struct VirtualConsole {
	    using this_type = VirtualConsole<UG,UL>;
	    using ccmdargs_type = ccmdargs_t<UG,UL>;
	    using int (&ccmd_type)(ccmdargs_type&);
	    using ccmdmap = std::unordered_map<u32string, ccmd_type*>;
		private:
		    UL _null_userlocal;
		public:
		    ccmdmap* ccmds = nullptr;
		    UG userglobal;

		    u32strlist outlns = {};

		    size_t max_outlines = 100;
		    size_t line_maxchars = 250;
            int write_raw(const u32string& s)
            {
                int linecount = 0;
                u32strlist lines2add = {};
                ezstr::split<char32_t>(lines2add, s, U'\n');
                //u32string cline; cline.reserve(this->line_maxchars);
                for (auto& cline : lines2add)
                {
	            	for (size_t c = 0; c < cline.size(); c += this->line_maxchars)
	            	{
	            		this->outlns.push_back(sub);
	            		linecount++;
	            		if (this->outlns.size() > this->max_outlines)
	            		{
	            			this->outlns.pop_front();
	            		}
	            	}
	            }
            	return linecount;
            }
		    int write_raw_u8(const std::string& s8)
		    {
		    	u32string u32; ezstr::mb2wide<char32_t>(u32, s8);
		    	return this->write_raw(u32);
		    }

		    int exec_parsed(const u32string& cn, const u32string& ca, UL& userlocal=_null_userlocal)
		    {
		    	if (this->ccmds == nullptr){return -1;}
		    	else if (this->ccmds->count(cn) == 0){return -2;}
		    	else {
		    		cmdargs_type cargs = {
		    			.vcon=*this,
		    			.args=ca,
		    			.userglobal=this->userglobal,
		    			.userlocal=userlocal
		    		};

		    		return this->ccmds[0][cn](cargs);
		    	}
		    }


	};
    /*
    class VirconInputHandler {
    	private:
    	    _inevtlist_t _pending_inevents = {};
    	public:
    	    u32strlist history = {U""};
    	    int cursor = 0, curhistidx = 0;
    	    void _process_pending_inevents()
    	    {
    	    	while (this->_pending_inevents->size() > 0)
    	    	{
    	    		const ConsoleInputEvent e = this->_pending_inevents->back();
    	    		this->_pending_inevents->pop_back();
    	    		switch (e.iclass)
    	    		{
    	    		    case vcon_inputclass::character:
    	    		    {
    	    		    	this->history[0].
    	    		    }
    	    			default: ;
    	    		}
    	    	}
    	    }

    }
	*/

}};
#endif

#if (defined(DJUTIL_NEEDS_binio) && !defined(__DJUTIL_H_binio_LOADED))
#define __DJUTIL_H_binio_LOADED
namespace djutil {namespace binio {
    const bool _hostIsLE = (*((const uint16_t*)"\1\0") == 1);

    template <class T>
    void bswap(T& vref)
    {
    	T tmp = vref;
    	std::reverse_copy((const char*)&tmp, (const char*)((&tmp)+1), (char*)&vref);
    }

    template <class VT, class RT=VT>
    void unpack(VT* out, std::istream& in, const size_t n=1, const bool le=_hostIsLE)
    {
    	RT invalue;
    	const size_t esz = sizeof(RT);
    	for (size_t i = 0; i < n; i++)
    	{
    		VT& outref = out[i];
    		const size_t curpos = in.tellg();
    		size_t nr = in.read((char*)&invalue, esz).gcount();
    		if (nr != esz)
    		{
    			std::stringstream errss;
    			errss << "Expected " << esz << " byte(s) starting from pos " << curpos << ", got " << nr << " byte(s) instead.\n";
    			throw std::runtime_error(errss.str());
    		}
    		if (le != _hostIsLE){bswap(invalue);}
    		outref = VT(invalue);
    	}
    }

    template <class VT, class WT=VT>
    size_t pack(std::ostream& out, const VT* in, const size_t n=1, const bool le=_hostIsLE)
    {
    	const size_t esz = sizeof(WT);
    	for (size_t i = 0; i < n; i++)
    	{
    		WT outval = WT(in[i]);
    		if (le != _hostIsLE){bswap(outval);}
    		out.write((const char*)&outval, esz);
    	}
    	return esz * n;
    }

}};
#endif

#if (defined(DJUTIL_NEEDS_ndarray) && !defined(__DJUTIL_H_ndarray_LOADED))
#define __DJUTIL_H_ndarray_LOADED
namespace djutil {namespace ndarray {

	#include <algorithm>
	#include <vector>

    const size_t NPOS = -1;

	using arrdims_t = std::vector<size_t>;

	using ndindex_t = std::vector<int>;

	enum class ndarraytype : int {
		normal = 0,
		view = 1
	};

	size_t _ndi2flat(const arrdims_t& dims, const arrdims_t& stride, const ndindex_t& ndi)
	{
		size_t fidx = 0;
		for (size_t n1 = 0, n2 = 1; n1 < dims.size() && n2)
		return fidx;
	}

	template <class DT>
	class ndarray {
	    public:
	        using dtype = DT;
	        using this_type = ndarray<DT>;
	    private:
	        arrdims_t dims;
	        size_t nelems = 0, nbytes = 0, lsize = 0;
	        ndarraytype arraytype = ndarraytype::normal;

	        dtype* elems = nullptr;

	        arrdims_t vstride;

	        size_t _calc_elem_offs(const ndindex_t& ndi) const {
	        	switch (this->arraytype)
	        	{
	        		case ndarraytype::normal:
	        		{
	        			for (size_t n = 0, n2 = 1; n < ndi.size() && n < this->dims.size(); n++, n2++)
	        			{
	        				const size_t
	        			}
	        		}
	        	}
	        }
	    public:
	        inline ndarraytype array_type() const {return this->arraytype;}
	        inline size_t num_elems() const {return this->nelems;}
	        inline size_t arrsize() const {return this->nbytes;}

	        inline arrdims_t ndim() const {return this->dims;}


	};
}};
#endif

#if (defined(DJUTIL_NEEDS_uvm) && !defined(__DJUTIL_H_uvm_LOADED))
#define __DJUTIL_H_uvm_LOADED
namespace djutil {namespace uvm {
    #include <iostream>
    #include <algorithm>
    #include <unordered_map>
    #include <vector>
    #include <cmath>
    #include <exception>

    struct uvm_t;
    struct uvmthread_t;

    using uvmopcode_t = unsigned int;
    enum struct uvm_scalar_class : unsigned char {
        sint = 0,
    	uint = 1,
    	flt = 2,
    	ptr = 3
    };
    #pragma pack(1)
    struct uvmptr_t {
    	uint32_t page;
    	int32_t offset;
    };
    #pragma pack()

    #pragma pack(1)

    union uvm_valreg_t {
   	    uvmptr_t p[16];

   	    double f8[16];
   	    float  f4[32];

   	    uint8_t  u1[128];
   	    uint16_t u2[64];
   	    uint32_t u4[32];
   	    uint64_t u8[16];

   	    int8_t  i1[128];
   	    int16_t i2[64];
   	    int32_t i4[32];
   	    int64_t i8[16];
   	};
    #pragma pack()

    #define UVM_REGISTER_MAX_SIZE 4096
    struct uvmregptr_t {
    	uint8_t* dataptr;
    	size_t datasize;
    };

    enum struct uvm_signal : int {
            sigjmp = 2, //regs.ip was manually modified by an instruction so don't automatically advance it.
            sigok = 1, //instruction executed successfully.
    	    sigterm = 0, //exit with success, returned by the term instruction
    	    sigsegv = -1, //VM segfault; attempted to access an invalid area of virtual memory
    	    sigill = -2, //illegal opcode.
    	    sigabrt = -3, //exit with error.
    	    sigidiv0 = -4 //integer division by zero.
    };

    class vm_exception : public std::exception {
        private:
            std::string _what = "";
    	public:
    	    uvm_signal sig;
    	    vm_exception(const uvm_signal _sig=uvm_signal::sigterm) : sig(_sig) {
    	        this->_what = "uvm signal " + std::to_string((int)this->sig);
    	    }
    	    virtual ~vm_exception() throw() {}
    	    virtual const char* what() const throw() {
    	    	return this->_what.c_str();
    	    }
    };

    struct uvmregs_t {
    	uvm_valreg_t vma, vml, vmr, vrt;
    	uvmptr_t ip;

    	uint8_t
    	    gpr1[UVM_REGISTER_MAX_SIZE],
    	    gpr2[UVM_REGISTER_MAX_SIZE],
    	    gpr3[UVM_REGISTER_MAX_SIZE],
    	    gpr4[UVM_REGISTER_MAX_SIZE]
    	;
    	uvmptr_t ptr1, ptr2;
    	uint8_t bvr1, bvr2;
    };

    bool uvm_get_reg_ptr(uvmregptr_t& rp, uvmregs_t& regs, const uint16_t idx)
    {
    	switch (idx)
    	{
    		case 0: rp = uvmregptr_t{.dataptr=regs.vma.u1, .datasize=sizeof(regs.vma.u1)}; return true;
    		case 1: rp = uvmregptr_t{.dataptr=regs.vml.u1, .datasize=sizeof(regs.vml.u1)}; return true;
    		case 2: rp = uvmregptr_t{.dataptr=regs.vmr.u1, .datasize=sizeof(regs.vmr.u1)}; return true;
    		case 3: rp = uvmregptr_t{.dataptr=regs.vrt.u1, .datasize=sizeof(regs.vrt.u1)}; return true;
    		case 4: rp = uvmregptr_t{.dataptr=(uint8_t*)&regs.ip, .datasize=sizeof(regs.ip)}; return true;
    		case 5: rp = uvmregptr_t{.dataptr=regs.gpr1, .datasize=sizeof(regs.gpr1)}; return true;
    		case 6: rp = uvmregptr_t{.dataptr=regs.gpr2, .datasize=sizeof(regs.gpr2)}; return true;
    		case 7: rp = uvmregptr_t{.dataptr=regs.gpr3, .datasize=sizeof(regs.gpr3)}; return true;
    		case 8: rp = uvmregptr_t{.dataptr=regs.gpr4, .datasize=sizeof(regs.gpr4)}; return true;

    		case 9: rp = uvmregptr_t{.dataptr=(uint8_t*)&regs.ptr1, .datasize=sizeof(regs.ptr1)}; return true;
    		case 10: rp = uvmregptr_t{.dataptr=(uint8_t*)&regs.ptr2, .datasize=sizeof(regs.ptr2)}; return true;
    		case 11: rp = uvmregptr_t{.dataptr=(uint8_t*)&regs.bvr1, .datasize=sizeof(regs.bvr1)}; return true;
    		case 12: rp = uvmregptr_t{.dataptr=(uint8_t*)&regs.bvr2, .datasize=sizeof(regs.bvr2)}; return true;
    		default: return false;
    	}
    }
    /*
    enum class regmov_subcmd : unsigned char {
    	dst = 0,
    	src = 1,
    	num = 2
    };
    */
    #pragma pack(1)
    struct _uvminstr_t {
    	uvmopcode_t opcode : 8;
    	union {
    	    char _remaining_24_bits[3];
    	    struct {
    	        unsigned char
    	            scalar_class : 3,
    	        	scalar_order : 2,
    	        	unused_bits1 : 3,

    	        	numscalars : 4,
    	        	unary_operation : 1,
    	        	r2l_operands : 1,
    	        	unused_bits2 : 2,

    	        	arithop : 8
    	        ;
    	    } varith;

    	    struct {
    	    	unsigned char _pad1;
    	    	union {
    	    	    uint16_t raw;
    	    	    struct {uint8_t lo : 8, hi : 8;};
    	    	} hcallno;
    	    } hostcall;

    	    struct {
    	    	uint8_t
    	    	    dst : 6,
    	    	    src : 6,
    	    	    val_lo : 4,
    	    	    val_hi : 8
    	    	;
    	    } regmov;

    	    struct {
    	    	uint8_t
    	    	    value_class : 3,
    	    	    value_order : 2,
    	    	    elemidx : 4
    	    	;
    	    	int8_t base : 7, exponent : 8;
    	    } vrtinc;

    	    struct {
    	        union {
    	    	    struct {uint8_t subopcode : 4, _unused1 : 4, _unused2[2];} header;
    	    	    struct {
    	    	    	uint8_t subopcode : 4;
    	    	    	bool is_unsigned_integer : 1;
    	    	    	uint8_t integer_order : 2, _unused1 : 1;
    	    	    	uint8_t srcvr : 6, dstpr : 6;
    	    	    	uint8_t srcvrelem : 4;
    	    	    } addr2ptr;
    	    	    struct {
    	    	    	uint8_t subopcode : 4;
    	    	    	bool is_unsigned_integer : 1;
    	    	    	uint8_t integer_order : 2, _unused1 : 1;
    	    	    	uint8_t srcpr : 6, dstvr : 6;
    	    	    	uint8_t dstvrelem : 4;
    	    	    } ptr2addr;
    	    	    struct {
    	    	    	uint8_t subopcode : 4, _unused1 : 1;
    	    	    	uint8_t dstptr_r : 6, regno : 6;
    	    	     } getregptr;
    	    	};
    	    } ptrconv;

    	    struct {
    	        uint8_t
    	            dstptr_r : 6,
    	            srcptr_r : 6,
    	            num_u4_r : 6,
    	            num_u4_idx : 4
    	        ;
    	        bool reverse_copy_flag : 1;
    	    } mov;

    	    struct {
    	        union {
    	    	    struct {uint8_t subopcode : 4, _unused1 : 4, _rest[2];} header;
    	    	    struct {
    	    	        uint8_t
    	    	            subopcode : 4,
    	    	            dstptr_r : 6,
    	    	            fillbyte_r : 6,
    	    	            numbytes_r : 6
    	    	        ;
    	    	    } fill;
    	    	};

    	    } fill;

    	    struct {
				uint8_t
				    subopcode : 4,
				    vma_idx : 4, //left op idx
				    vmr_idx : 4, //right op idx
				    value_class : 3,
				    value_order : 2
				;
				bool put_result_in_bvr2_flag : 1;
				uint8_t _unused1 : 6;
			} vcmp;
			struct {
				uint8_t logic_op : 8;
				bool bvr1_to_bvr2_flag : 1;
				uint8_t _unused1 : 8, _unused2 : 7;
			} blogic;
            struct {
                bool cond_is_bvr2_flag : 1;
                uint16_t
                    iftrue_nisrs_lo : 8,
                    iftrue_nisrs_hi : 8,
                    _unused1 : 7
                ;
            } branch;
            struct {
                uint32_t
                    dstidx_lo : 8,
                    dstidx_med : 8,
                    dstidx_hi : 8
                ;
            } _goto;

            struct {
                uint8_t
                    chr_order : 2,
                    chr_reg : 6
                ;
                uint16_t
                    chr_idx_lo : 4,
                    chr_idx_hi : 8
                ;
                uint8_t _unused1 : 4;
            } putchr;
    	};
    };
    #pragma pack()

    enum struct uvm_vcmp_subopcode : uint8_t {
		// a = vma[vma_idx]
		// b = vma[vmr_idx]

		eq = 0, // a == vmr
		ne = 1, // a != b

		lt = 2, // a < b
		le = 3, // a <= b

		gt = 4, // a > b
		ge = 5, // a >= b


		truths_or = 6, // bool(a) || bool(b)
		truths_and = 7, // bool(a) && bool(b)
	};

    enum struct uvm_ptrconv_valregs : uint8_t {
    	vma = 0,
    	vml = 1,
    	vmr = 2,
    	vrt = 3
    };

    enum struct uvm_ptrconv_ptrregs : uint8_t {
    	ptr1 = 0,
    	ptr2 = 1
    };

    struct uvmcalldata_t {
    	uvm_t* vm;
    	//uvmthread_t* thr;
    };

    using _uvmopfunc_t = int(*)(uvmcalldata_t&);



    #define _DJUTIL_UVM_OPCODES_M(_opcf, ...) \
        _opcf(0, term, __VA_ARGS__) \
        _opcf(1, hostcall, __VA_ARGS__) \
        _opcf(2, varith, __VA_ARGS__) \
        _opcf(3, regmov, __VA_ARGS__ ) \
        _opcf(4, vrtinc, __VA_ARGS__ ) \
        _opcf(5, ptrconv, __VA_ARGS__ ) \
        _opcf(6, mov, __VA_ARGS__ ) \
        _opcf(7, fill, __VA_ARGS__ ) \
        _opcf(8, vcmp, __VA_ARGS__ ) \
        _opcf(9, blogic, __VA_ARGS__ ) \
        _opcf(10, branch, __VA_ARGS__ ) \
        _opcf(11, _goto, __VA_ARGS__ ) \
        _opcf(12, putchr, __VA_ARGS__ ) \
    \

    #define _djutil_uvm_opcf0(_c, _n, ...) \
    int _uvmopfunc_ ## _n(uvmcalldata_t&); \
    \

    _DJUTIL_UVM_OPCODES_M(_djutil_uvm_opcf0)

    #undef _djutil_uvm_opcf0

    #define _djutil_uvm_opcf1(_c, _n, ...) _uvmopfunc_ ## _n,

    _uvmopfunc_t _UVM_DISPATCH_TABLE[256] = {
        _DJUTIL_UVM_OPCODES_M(_djutil_uvm_opcf1)
    };

    #undef _djutil_uvm_opcf1

    #define _djutil_uvm_opcf2(_c, _n, ...) static const uvmopcode_t _n = _c;

    struct uvm_opcodes {
        _DJUTIL_UVM_OPCODES_M(_djutil_uvm_opcf2)
    };

    #undef _djutil_uvm_opcf2
    #define _UVM_VMEM_MAXADDR 8388608U
    #define _UVM_MAX_MEMCELLS 2048U
    #define _UVM_MEMCELL_SIZE (_UVM_VMEM_MAXADDR / _UVM_MAX_MEMCELLS)
    const inline uint32_t _addr2cell(const uint32_t a)
    {
    	return a/_UVM_MEMCELL_SIZE;
    }

    struct uvmmemcell_t;
    struct uvmpage_t {
        uint32_t idx;
    	bool
    	    r : 1,
    	    w : 1,
    	    x : 1
    	;
    	uint32_t vmstartaddr, vmendaddr;
    	uint32_t size;
    	uint8_t* data;
    	bool owns_data;
    	uint32_t codestartofs, codeendofs;
    };
    struct uvmmemcell_t {
        uint32_t cellnum = 0;
        uint32_t cellstart = 0, cellend = 0;
    	std::vector<uint32_t> overlapping_pages = {};
    };

    template <class T>
    void _varith_func(uvmregs_t& regs, const unsigned char op, const bool r2l, const bool unaryop, const size_t n)
    {
        T* a = reinterpret_cast<T*>(regs.vma.u1);
        T* l = (unaryop ? a : reinterpret_cast<T*>((r2l ? regs.vmr.u1 : regs.vml.u1)));
        T* r = reinterpret_cast<T*>((r2l ? regs.vml.u1 : regs.vmr.u1));
    	switch (op)
    	{
    		case '+': for (size_t i = 0; i < n; i++){a[i] = l[i] + r[i];} return;
    		case '-': for (size_t i = 0; i < n; i++){a[i] = l[i] - r[i];} return;
    		case '*': for (size_t i = 0; i < n; i++){a[i] = l[i] * r[i];} return;
    		case '/': for (size_t i = 0; i < n; i++){a[i] = l[i] / r[i];} return;
    		default: return;
    	}
    }

    template <class IT>
    void _vparith_func(uvmregs_t& regs, const unsigned char op, const bool r2l, const bool unaryop, const size_t n)
    {
    	uvmptr_t
    	    *a = regs.vma.p,
    	    *l = regs.vml.p
    	;
    	IT* r = reinterpret_cast<IT*>(regs.vmr.u1);

    	switch (op)
    	{
    		case '+': for (size_t i = 0; i < n; i++){a[i] = l[i]; a[i].offset += r[i];} return;
    		case '-': for (size_t i = 0; i < n; i++){a[i] = l[i]; a[i].offset -= r[i];} return;
    		default: return;
    	}
    }

    using _varithfunc_t = void (*)(uvmregs_t&, const unsigned char, const bool, const bool, const size_t);
    _varithfunc_t _UVM_VARITH_FUNCS[4][4] = {
    	{_varith_func<int8_t>, _varith_func<int16_t>, _varith_func<int32_t>, _varith_func<int64_t>},
    	{_varith_func<uint8_t>, _varith_func<uint16_t>, _varith_func<uint32_t>, _varith_func<uint64_t>},
    	{nullptr, nullptr, _varith_func<float>, _varith_func<double>},
    	{_vparith_func<int8_t>, _vparith_func<int16_t>, _vparith_func<int32_t>, _vparith_func<int64_t>}
    };

    template <class VT>
    uvm_signal _vcmpfunc1(uvmregs_t& regs, const uvm_vcmp_subopcode op, const size_t l_idx, const size_t r_idx, const bool in_bvr2) {
		VT& lv = ((VT*)regs.vma.u1)[l_idx];
		VT& rv = ((VT*)regs.vmr.u1)[r_idx];
		bool res = false;
		switch (op) {
			case uvm_vcmp_subopcode::eq: res = (lv == rv); break;
			case uvm_vcmp_subopcode::ne: res = (lv != rv); break;
			case uvm_vcmp_subopcode::lt: res = (lv < rv); break;
			case uvm_vcmp_subopcode::le: res = (lv <= rv); break;
			case uvm_vcmp_subopcode::gt: res = (lv > rv); break;
			case uvm_vcmp_subopcode::ge: res = (lv >= rv); break;

			case uvm_vcmp_subopcode::truths_or: res = (bool(lv) || bool(rv)); break;
			case uvm_vcmp_subopcode::truths_and: res = (bool(lv) && bool(rv)); break;
			default: return uvm_signal::sigill;
		}
		uint8_t& dstreg = *(in_bvr2 ? &regs.bvr2 : &regs.bvr1);
		dstreg = (res ? uint8_t(1) : uint8_t(0));
		return uvm_signal::sigok;
	}

    using _vcmpfunc_t = uvm_signal(*)(uvmregs_t&, const uvm_vcmp_subopcode, const size_t, const size_t, const bool);

    _vcmpfunc_t _VCMP_SUBOP_FUNCS[4][4] = {
		{_vcmpfunc1<int8_t>, _vcmpfunc1<int16_t>, _vcmpfunc1<int32_t>, _vcmpfunc1<int64_t>},
		{_vcmpfunc1<uint8_t>, _vcmpfunc1<uint16_t>, _vcmpfunc1<uint32_t>, _vcmpfunc1<uint64_t>},
		{nullptr, nullptr, _vcmpfunc1<float>, _vcmpfunc1<double>},
		{nullptr, nullptr, nullptr, nullptr}
	};

    static const char _UVMXFF_IDENTIFIER[8] = {'U', 'V', 'M', 'x', 'f', 'f', '\n', '\0'};
    struct UVMXFF_ReadHeader {
    	char id[8];
    	uint32_t fileversion, numstrings, numsections;
    };
    struct UVMXFF_ReadString {
    	uint16_t len;
    	std::string str;
    };
    struct UVMXFF_ReadSection {
    	char id[4];
    	uint32_t datalen;
    	size_t datastart, dataend;
    };
    class UVMXFF_ReadData {
        private:
            std::istream* _istream = nullptr;
            UVMXFF_ReadHeader _header = {};
            std::vector<UVMXFF_ReadString> _strings = {};
            std::vector<UVMXFF_ReadSection> _sections = {};
            std::unordered_map<std::string, uint32_t> _sectids_byname = {};
        public:
            std::istream* const& istr = _istream;
            const UVMXFF_ReadHeader& header = _header;
            const std::vector<UVMXFF_ReadString>& strings = _strings;
            const std::vector<UVMXFF_ReadSection>& sections = _sections;
            const std::unordered_map<std::string, uint32_t>& sectids_byname = _sectids_byname;
            void close() {
            	this->_istream = nullptr;
            	this->_header = UVMXFF_ReadHeader{};
            	this->_strings.clear();
            	this->_sections.clear();
            	this->_sectids_byname.clear();
            }
  	    	size_t load(std::istream& xff) {
  	    	    this->close();
		    	auto& hdr = this->_header;
		    	this->_istream = &xff;
		    	auto startpos = xff.tellg();
		    	if (xff.read(hdr.id, 8).gcount() < 8 || memcmp(hdr.id, _UVMXFF_IDENTIFIER, 8) != 0)
		    	{
		    		throw std::runtime_error("uvmxff file identifier string not found.");
		    	}

		        binio::unpack(&hdr.fileversion, xff, 3, true);

		    	if (hdr.fileversion != 0)
		    	{
		    		throw std::runtime_error("bad uvmxff file version: "+std::to_string(hdr.fileversion));
		    	}

		    	this->_strings.resize(hdr.numstrings);
		    	uint32_t strindex = 0;
		    	for (auto& str : this->_strings)
		    	{
		    		binio::unpack(&str.len, xff, 1, true);
		    		str.str.resize(str.len);
		    		binio::unpack(&str.str.front(), xff, str.len);
		    		std::cout << "stringlist[" << strindex << "] = " << std::quoted(str.str) << ";\n";
		    		strindex++;
		    	}



		    	//std::vector<_uvmxff_section> sections = {}; sections.resize(hdr.numsections);
		    	//std::unordered_map<std::string, uint32_t> named_sections;
		    	this->_sections.resize(hdr.numsections);

		    	for (uint32_t i = 0; i < hdr.numsections; i++)
		    	{
		    	    auto& sect = this->_sections[i];
		    	    binio::unpack(sect.id, xff, 4);
		    	    binio::unpack(&sect.datalen, xff, 1, true);
		    		sect.datastart = xff.tellg();
		    		xff.seekg(sect.datastart + sect.datalen);
		    		sect.dataend = xff.tellg();
		    		std::string idstr(sect.id, 4);
		    		if (this->_sectids_byname.count(idstr) == 0)
		    		{
		    		    this->_sectids_byname[idstr] = i;
		    		}
		    	}

		    	auto endpos = xff.tellg();
		    	return size_t(endpos) - size_t(startpos);
	    	}

	    	UVMXFF_ReadData() {}
	    	~UVMXFF_ReadData() {this->close();}
    };

    struct uvmxff_loadres {
    	uvmptr_t symtblstart, textstart, datastart, codestart;
    	uint32_t symtblsize, textsize, datasize, codesize;
    };

    using uvmhostcall_t = int (*)(uvmcalldata_t&);
    struct uvm_t {
        uint32_t _cur_pageno = 1;
        std::unordered_map<uint32_t, uvmmemcell_t> active_memcells = {};
        std::unordered_map<uint32_t, uvmpage_t> pagetable = {};
        std::unordered_map<uint16_t, uvmhostcall_t> hostcalls = {};
        uvmptr_t regptrs[64] = {};
        _uvminstr_t* curinstr = nullptr;
        uvmregs_t regs = {};

        uint16_t regmov_curdst = 0, regmov_cursrc = 0;
        void *user1 = nullptr, *user2 = nullptr;
        bool getpage4idx(uvmpage_t*& out, const uint32_t idx)
        {
        	if (this->pagetable.count(idx) == 0){return false;}
        	out = &(this->pagetable[idx]);
        	return true;
        }
        bool getcell4idx(uvmmemcell_t*& out, const uint32_t idx)
        {
            if (this->active_memcells.count(idx) == 0){return false;}
            out = &(this->active_memcells[idx]);
            return true;
        }
        bool addr2ptr(uvmptr_t& p, const uint32_t a)
        {
        	uint32_t cellnum = _addr2cell(a);
        	if (this->active_memcells.count(cellnum) == 0){return false;}
        	for (auto& pagenum : this->active_memcells[cellnum].overlapping_pages)
        	{
        		auto& page = this->pagetable[pagenum];
        		if (a >= page.vmstartaddr && a < page.vmendaddr)
        		{
        			p.page = pagenum;
        			p.offset = a - page.vmstartaddr;
        			return true;
        		}
        	}
        	return false;
        }

        uint32_t newpage(uint8_t* buffer, const uint32_t vmstartaddr, const uint32_t size, const bool do_overlap_check=true, const bool owns_buffer=false)
        {
        	uint32_t newid = this->_cur_pageno;
        	if (size == 0){return 0;}
        	uint32_t vmendaddr = vmstartaddr + size;

        	uint32_t startcell = _addr2cell(vmstartaddr);
        	uint32_t endcell = _addr2cell(vmendaddr);

        	uvmmemcell_t* curcell_p = nullptr;
        	uvmpage_t* page_p = nullptr;

        	if (do_overlap_check)
        	{
        		for (uint32_t cell = startcell; cell <= endcell; cell++)
        		{
        			if (this->getcell4idx(curcell_p, cell))
        			{
        				for (auto& pageno : curcell_p->overlapping_pages)
        				{
        					if (this->getpage4idx(page_p, pageno))
        					{
        						if ((vmstartaddr >= page_p->vmstartaddr && vmstartaddr < page_p->vmendaddr) || (vmendaddr >= page_p->vmstartaddr && vmendaddr < page_p->vmendaddr))
        						{
        							return 0;
        						}
        					}
        				}
        			}
        		}
        	}

        	this->pagetable[newid] = uvmpage_t{
        	    .idx=newid,
        	    .r=true, .w=true, .x=true,
        	    .vmstartaddr=vmstartaddr,
        	    .vmendaddr=vmendaddr,
        	    .size=size,
        	    .data=buffer,
        	    .owns_data=owns_buffer,
        	    .codestartofs=0,
        	    .codeendofs=0
        	};

        	for (uint32_t cell = startcell; cell <= endcell; cell++)
        	{
        		if (!this->getcell4idx(curcell_p, cell))
        		{
        			this->active_memcells[cell] = uvmmemcell_t{
        				.cellnum=cell,
        				.cellstart=cell * _UVM_MEMCELL_SIZE,
        				.cellend=(cell + 1) * _UVM_MEMCELL_SIZE,
        				.overlapping_pages={newid}
        			};
        		}
        		else
        		{
        			curcell_p->overlapping_pages.push_back(newid);
        		}
        	}

        	this->_cur_pageno++;
        	return newid;
        }

	    void delpage(const uint32_t pageidx)
	    {
	    	uvmpage_t* page = nullptr;
	    	if (!this->getpage4idx(page, pageidx)){return;}

	    	uint32_t startcell = _addr2cell(page->vmstartaddr);
	    	uint32_t endcell = _addr2cell(page->vmendaddr);

	    	for (uint32_t cell = startcell; cell <= endcell; cell++)
	    	{
	    		uvmmemcell_t* mcell = nullptr;
	    		if (this->getcell4idx(mcell, cell))
	    		{
	    			bool foundit = false;
	    			uint32_t fidx = 0;
	    			for (; fidx < mcell->overlapping_pages.size(); fidx++)
	    			{
	    				if (mcell->overlapping_pages[fidx] == pageidx)
	    				{
	    					foundit = true;
	    					break;
	    				}
	    			}
	    			if (foundit){mcell->overlapping_pages.erase(mcell->overlapping_pages.begin() + fidx);}
	    			if (mcell->overlapping_pages.size() == 0){this->active_memcells.erase(cell);}
	    		}

	    	}
	    	if (page->owns_data && page->data != nullptr)
	    	{
	    		delete[] page->data;
	    		page->data = nullptr;
	    	}
	    	this->pagetable.erase(pageidx);
	    	page = nullptr;

	    }
	    void* ptrderef(const uvmptr_t& ptr, const bool throws_sigsegv_if_bad=false)
	    {
	        uvmpage_t* page = nullptr;
	        if (ptr.page > 0 && this->getpage4idx(page, ptr.page))
	        {
	        	return (void*)(page->data + ptr.offset);
	        }
	        else if (throws_sigsegv_if_bad) {
	        	throw vm_exception(uvm_signal::sigsegv);
	        }
	        else {return nullptr;}
	    }
	    _uvmopfunc_t fetchcur()
	    {
	    	this->curinstr = (_uvminstr_t*)this->ptrderef(this->regs.ip);
	    	return _UVM_DISPATCH_TABLE[this->curinstr->opcode];
	    }
	    void handle_vm_exception(const uvm_signal sig) {
	    	throw std::runtime_error("VM abort from unhandled exception: " + std::to_string(int(sig)));
	    }
	    int execstart(){
	        uvmcalldata_t cd = {.vm=this};
	        _uvminstr_t*& ci = this->curinstr;
	        uvmptr_t& ip = this->regs.ip;
	        int callres = 0;
	    	//while (_UVM_DISPATCH_TABLE[(ci = (_uvminstr_t*)this->ptrderef(ip))->opcode](cd) != 0){
	    	while (true){
	    	    callres = 0;
	    	    this->curinstr = (_uvminstr_t*)this->ptrderef(ip);
	    	    if (this->curinstr == nullptr){callres = -1;}
	    	    else if (this->curinstr->opcode >= (sizeof(_UVM_DISPATCH_TABLE)/sizeof(_UVM_DISPATCH_TABLE[0]))){
	    	        callres = int(uvm_signal::sigill);
	    	    }
	    	    else {
	    	        callres = _UVM_DISPATCH_TABLE[this->curinstr->opcode](cd);
	    	    }

	    	    if (callres < 0){
	    	        std::cout << "got exception: " << callres << '\n';
	    	        this->handle_vm_exception(uvm_signal(callres));
	    	    }
	    	    else if (callres == 2){continue;}
	    	    else if (callres == 1){ip.offset += 4;}
	    	    else if (callres == 0){break;}
	    	}
	    	return 0;
	    }
	    uint32_t mmap_regs()
	    {
	    	uint32_t cur_addr = 0x8;
	    	uvmregptr_t rp;
	    	for (uint32_t regnum = 0; uvm_get_reg_ptr(rp, this->regs, regnum); regnum++)
	    	{
	    		uint32_t pageid = this->newpage(rp.dataptr, cur_addr, rp.datasize, true);

	    		if (pageid == 0)
	    		{
	    			throw std::runtime_error("mmap_regs() failure.");
	    		}
	    		this->regptrs[regnum] = uvmptr_t{.page=pageid, .offset=0};
	    		cur_addr += rp.datasize;
	    	}
	    	return cur_addr;
	    }

	    size_t load_raw_bytecode(uvmptr_t& codestart, std::istream& bcf, const uint32_t loadaddr=-1)
	    {
	    	size_t codesize = 0;
	    	uint32_t load_addr = loadaddr;
	    	if (load_addr == uint32_t(-1))
	    	{
	    	    load_addr = 0;
		    	for (const auto& ipp : this->pagetable)
		    	{
		    		load_addr = std::max(load_addr, ipp.second.vmendaddr);
		    	}
		    }
		    auto startpos = bcf.tellg();
		    bcf.seekg(0, std::ios::end);
		    codesize = size_t(bcf.tellg() - startpos);
		    bcf.seekg(startpos);
		    if (codesize == 0 || codesize % 4 != 0){return 0;}
		    uint8_t* codebuf = new uint8_t[codesize];
	    	bcf.read((char*)codebuf, codesize);

	    	uint32_t page = this->newpage(codebuf, load_addr, codesize, true, true);

	    	codestart = uvmptr_t{.page=page, .offset=0};
	    	uvmpage_t* pageent = nullptr;
	    	this->getpage4idx(pageent, page);
	    	pageent->codestartofs = 0;
	    	pageent->codeendofs = pageent->codestartofs + codesize;
	    	return codesize;
	    }

	    uvmxff_loadres load_uvmxff(std::istream& xff)
	    {
	        uvmxff_loadres res = {};
	        UVMXFF_ReadData rd; rd.load(xff);
	        auto oldpos = xff.tellg();
	        uint32_t load_addr = 4;
	        for (const auto& ipp : this->pagetable)
	        {
	        	load_addr = std::max(load_addr, ipp.second.vmendaddr);
	        }

	        const UVMXFF_ReadSection& codesect = rd.sections[rd.sectids_byname.at("CODE")];
	        uint32_t totalsz = 0;
	        totalsz += codesect.datalen;
	        uint8_t* program = new uint8_t[totalsz];
	        xff.seekg(codesect.datastart);
	        xff.read((char*)program, codesect.datalen);

	        uint32_t pageid = this->newpage(program, load_addr, totalsz, true, true);
	        res.codestart.page = pageid;
	        res.codestart.offset = 0;
	        res.codesize = codesect.datalen;
	        xff.seekg(oldpos);
	        rd.close();

	        uvmpage_t* page = nullptr;
	        this->getpage4idx(page, pageid);
	        page->codestartofs = res.codestart.offset;
	        page->codeendofs = page->codestartofs + res.codesize;
	        return res;
	    }
	    bool _reljmp(const uint32_t dstisridx) {
			//ANY INSTRUCTIONS THAT CALL THIS MUST SIGNAL 2 IN PLACE OF SIGNAL 1
			//OR ELSE THE VM WILL INCREMENT IP BY ANOTHER INSTRUCTION!
			uvmptr_t newip = this->regs.ip;
			uvmpage_t* curpage = nullptr;
			if (!this->getpage4idx(curpage, newip.page)){return false;}
			newip.offset = curpage->codestartofs;
			newip.offset += (dstisridx * 4);
			this->regs.ip = newip;
			return true;
		}
	};
    /*
    struct uvmthread_t {
    	uvmregs_t regs;
    };
    */
    #undef _DJUTIL_UVM_OPCODES_M

    int _uvmopfunc_term(uvmcalldata_t& c) {return 0;}
    int _uvmopfunc_hostcall(uvmcalldata_t& c) {
        auto& ip = *c.vm->curinstr;
        uint16_t hcallno = ip.hostcall.hcallno.raw;
        if (!binio::_hostIsLE){
        	hcallno = ((ip.hostcall.hcallno.hi << 8) & 0xFF00U) | (ip.hostcall.hcallno.lo & 0xFFU);
        }
        return c.vm->hostcalls.at(hcallno)(c);
    }
    int _uvmopfunc_varith(uvmcalldata_t& c) {
        uvm_t* vm = c.vm;
        uvmregs_t& regs = vm->regs;
        auto& ip = *(vm->curinstr);
        _UVM_VARITH_FUNCS[ip.varith.scalar_class][ip.varith.scalar_order](regs, ip.varith.arithop, ip.varith.r2l_operands, ip.varith.unary_operation, ip.varith.numscalars+1);
    	return 1;
    }
    int _uvmopfunc_regmov(uvmcalldata_t& c) {
        uvm_t* vm = c.vm;
        uvmregs_t& regs = vm->regs;
        _uvminstr_t& ip = *(vm->curinstr);

        uint16_t val = (uint16_t(ip.regmov.val_hi & 0xFU) << 8);
        val |= (ip.regmov.val_lo & 0xFFU);

        uint16_t dsti = (ip.regmov.dst & 0b0111111U);
        uint16_t srci = (ip.regmov.src & 0b0111111U);

   	    if (dsti == srci || val == 0){return 1;}
   		uvmregptr_t dst, src;
   		if (uvm_get_reg_ptr(dst, regs, dsti) && uvm_get_reg_ptr(src, regs, srci))
   		{
   			std::copy_n(src.dataptr, val, dst.dataptr);
   		}
   		else
   		{
   			return int(uvm_signal::sigsegv);
   		}

    	return 1;
    }
    template <class T>
    void _uvm_vrtinc_func(uvm_valreg_t& vrt, const _uvminstr_t& ip)
    {
        T& v = ((T*)vrt.u1)[ip.vrtinc.elemidx];

        T base = (ip.vrtinc.base < 0 ? -ip.vrtinc.base : ip.vrtinc.base);

        T exponent = ip.vrtinc.exponent;
        if (base == 0 && exponent == 0){v = T(0); return;}

        bool subtracts = (ip.vrtinc.base < 0);
        T i = T(pow(base, exponent));

    	if (subtracts){v -= i;}
    	else          {v += i;}
    }
    int _uvmopfunc_vrtinc(uvmcalldata_t& c) {
        uvm_t* vm = c.vm;
        uvmregs_t& regs = vm->regs;
        _uvminstr_t& ip = *(vm->curinstr);
        using _vrtincfunc_t = void(*)(uvm_valreg_t&, const _uvminstr_t&);
        static _vrtincfunc_t _dtbl[4][4] = {
        	{_uvm_vrtinc_func<int8_t>, _uvm_vrtinc_func<int16_t>, _uvm_vrtinc_func<int32_t>, _uvm_vrtinc_func<int64_t>},
        	{_uvm_vrtinc_func<uint8_t>, _uvm_vrtinc_func<uint16_t>, _uvm_vrtinc_func<uint32_t>, _uvm_vrtinc_func<uint64_t>},
        	{nullptr, nullptr, _uvm_vrtinc_func<float>, _uvm_vrtinc_func<double>},
        	{}
        };
        //std::cout << "vrtinc\n";
        auto call = _dtbl[ip.vrtinc.value_class][ip.vrtinc.value_order];
        if (call != nullptr){call(regs.vrt, ip);}
        else {return int(uvm_signal::sigill);}
    	return 1;
    }
    using _uvm_ptrconv_func_t = int(*)(uvmcalldata_t&);

    template <class T>
    int _uvm_ptrconv_func_addr2ptr(uvmcalldata_t& c)
    {
        uvm_t* vm = c.vm;
        uvmregs_t& regs = vm->regs;
        _uvminstr_t& ip = *(vm->curinstr);
        uvmregptr_t ptr_r, addr_r;
        if (!uvm_get_reg_ptr(ptr_r, regs, ip.ptrconv.addr2ptr.dstpr)){return int(uvm_signal::sigsegv);}
        if (!uvm_get_reg_ptr(addr_r, regs, ip.ptrconv.addr2ptr.srcvr)){return int(uvm_signal::sigsegv);}
        T addr = ((T*)(addr_r.dataptr))[ip.ptrconv.addr2ptr.srcvrelem];
        vm->addr2ptr(*(uvmptr_t*)ptr_r.dataptr, uint32_t(addr));
    	return 1;
    }
    template <class T>
    int _uvm_ptrconv_func_ptr2addr(uvmcalldata_t& c)
    {
    	uvm_t* vm = c.vm;
        uvmregs_t& regs = vm->regs;
        _uvminstr_t& ip = *(vm->curinstr);

        uvmregptr_t ptr_r, addr_r;
        if (!uvm_get_reg_ptr(ptr_r, regs, ip.ptrconv.ptr2addr.srcpr)){return int(uvm_signal::sigsegv);}
        if (!uvm_get_reg_ptr(addr_r, regs, ip.ptrconv.ptr2addr.dstvr)){return int(uvm_signal::sigsegv);}
        T& addr = ((T*)(addr_r.dataptr))[ip.ptrconv.ptr2addr.dstvrelem];
    	uvmpage_t* page = nullptr;
    	uvmptr_t ptr = *(uvmptr_t*)ptr_r.dataptr;
    	if (vm->getpage4idx(page, ptr.page))
    	{
    		addr = T(page->vmstartaddr + ptr.offset);
    	}
    	else
    	{
    		addr = T(0);
    	}
    	return 1;
    }
    int _uvm_ptrconv_func_getregptr(uvmcalldata_t& c)
    {
    	auto* vm = c.vm;
    	auto& regs = vm->regs;
    	auto& ip = *(vm->curinstr);

    	uvmregptr_t dst, src;
    	if (!uvm_get_reg_ptr(dst, regs, ip.ptrconv.getregptr.dstptr_r))
    	{
    		return int(uvm_signal::sigsegv);
    	}
    	auto& dstp = *(uvmptr_t*)dst.dataptr;
    	auto regnum = ip.ptrconv.getregptr.regno;
    	dstp = vm->regptrs[regnum];
    	return 1;
    }
    _uvm_ptrconv_func_t _ptrconv_disptbl[3][2][4] = {
        {
            {_uvm_ptrconv_func_addr2ptr<int8_t>, _uvm_ptrconv_func_addr2ptr<int16_t>, _uvm_ptrconv_func_addr2ptr<int32_t>, _uvm_ptrconv_func_addr2ptr<int64_t>},
            {_uvm_ptrconv_func_addr2ptr<uint8_t>, _uvm_ptrconv_func_addr2ptr<uint16_t>, _uvm_ptrconv_func_addr2ptr<uint32_t>, _uvm_ptrconv_func_addr2ptr<uint64_t>}
        },
        {
            {_uvm_ptrconv_func_ptr2addr<int8_t>, _uvm_ptrconv_func_ptr2addr<int16_t>, _uvm_ptrconv_func_ptr2addr<int32_t>, _uvm_ptrconv_func_ptr2addr<int64_t>},
            {_uvm_ptrconv_func_ptr2addr<uint8_t>, _uvm_ptrconv_func_ptr2addr<uint16_t>, _uvm_ptrconv_func_ptr2addr<uint32_t>, _uvm_ptrconv_func_ptr2addr<uint64_t>}
        },
        {
            {_uvm_ptrconv_func_getregptr, _uvm_ptrconv_func_getregptr, _uvm_ptrconv_func_getregptr, _uvm_ptrconv_func_getregptr},
            {_uvm_ptrconv_func_getregptr, _uvm_ptrconv_func_getregptr, _uvm_ptrconv_func_getregptr, _uvm_ptrconv_func_getregptr}
        }
    };
    int _uvmopfunc_ptrconv(uvmcalldata_t& c) {
        uvm_t* vm = c.vm;
        if (vm == nullptr){std::cout << "ptrconv: vm is NULL!\n"; std::cout.flush();}
        if (vm->curinstr == nullptr){std::cout << "ptrconv: vm->curinstr is NULL!\n"; std::cout.flush();}
        uvmregs_t& regs = vm->regs;
        _uvminstr_t& ip = *(vm->curinstr);


        if (ip.ptrconv.header.subopcode > 2){
        	return int(uvm_signal::sigill);
        }
    	auto* callptr = _ptrconv_disptbl[ip.ptrconv.header.subopcode][int(ip.ptrconv.addr2ptr.is_unsigned_integer)][ip.ptrconv.addr2ptr.integer_order];
        if (callptr == nullptr){return int(uvm_signal::sigill);}
        return callptr(c);
    }

    int _uvmopfunc_mov(uvmcalldata_t& c)
    {
        uvm_t* vm = c.vm;
        uvmregs_t& regs = vm->regs;
        auto& ip = *(vm->curinstr);
        uvmregptr_t dst_r, src_r, num_r;
        if (!uvm_get_reg_ptr(dst_r, regs, ip.mov.dstptr_r & 0xFU)){return int(uvm_signal::sigsegv);}
        if (!uvm_get_reg_ptr(src_r, regs, ip.mov.srcptr_r & 0xFU)){return int(uvm_signal::sigsegv);}
        if (!uvm_get_reg_ptr(num_r, regs, ip.mov.num_u4_r & 0xFU)){return int(uvm_signal::sigsegv);}

        auto* dstaddr = (uint8_t*)vm->ptrderef(*(uvmptr_t*)dst_r.dataptr);
        auto* srcaddr = (uint8_t*)vm->ptrderef(*(uvmptr_t*)src_r.dataptr);
        if (dstaddr == nullptr || srcaddr == nullptr){return int(uvm_signal::sigsegv);}
        auto& num = ((uint32_t*)num_r.dataptr)[ip.mov.num_u4_idx];
        //std::cout << "mov_num = " << num << '\n';
        if (ip.mov.reverse_copy_flag){std::reverse_copy(srcaddr, srcaddr + num, dstaddr);}
        else {std::copy_n(srcaddr, num, dstaddr);}
      	return 1;
    }
    int _uvm_fill_sub0(uvmcalldata_t& c) {
        uvm_t* vm = c.vm;
        uvmregs_t& regs = vm->regs;
        auto& ip = *(vm->curinstr);
        uvmregptr_t dstptr_r, fillbyte_r, numbytes_r;
        uvm_get_reg_ptr(dstptr_r, regs, ip.fill.fill.dstptr_r);
        if (!uvm_get_reg_ptr(fillbyte_r, regs, ip.fill.fill.fillbyte_r)){return int(uvm_signal::sigsegv);}
        if (!uvm_get_reg_ptr(numbytes_r, regs, ip.fill.fill.numbytes_r)){return int(uvm_signal::sigsegv);}
        auto* dstptr = (uint8_t*)vm->ptrderef(*(uvmptr_t*)dstptr_r.dataptr);
        if (dstptr == nullptr){return int(uvm_signal::sigsegv);}
        auto& fillbyte = *(uint8_t*)fillbyte_r.dataptr;
        auto& numbytes = *(uint32_t*)numbytes_r.dataptr;

        memset(dstptr, fillbyte, numbytes);

    	return 1;
    }

    int _uvmopfunc_fill(uvmcalldata_t& c)
    {
    	uvm_t* vm = c.vm;
    	auto& ip = *(vm->curinstr);
    	static _uvmopfunc_t _subdtbl[2] = {
    		_uvm_fill_sub0,
    		 nullptr
    	};

    	auto* func = _subdtbl[ip.fill.header.subopcode];
    	if (func == nullptr){return int(uvm_signal::sigill);}
    	return func(c);
    }
    int _uvmopfunc_vcmp(uvmcalldata_t& c) {
		uvm_t* vm = c.vm;
		auto& ip = *(vm->curinstr);
		size_t vcls = ip.vcmp.value_class;
		size_t vord = ip.vcmp.value_order;
		if (vcls >= 4 || vord >= 4){return int(uvm_signal::sigill);}
		auto* func = _VCMP_SUBOP_FUNCS[vcls][vord];
		if (func == nullptr){return int(uvm_signal::sigill);}
		return int(func(vm->regs, uvm_vcmp_subopcode(ip.vcmp.subopcode), ip.vcmp.vma_idx, ip.vcmp.vmr_idx, ip.vcmp.put_result_in_bvr2_flag));
	}

	enum struct uvm_blogic_op : uint8_t {
		_or = 0, // (a | b) & 1
		_and = 1, // (a & b) & 1
		_nor = 2, // ~(a | b) & 1
		_nand = 3, // ~(a & b) & 1
		_xor = 4 // (a ^ b) & 1
	};

	int _uvmopfunc_blogic(uvmcalldata_t& c) {
		uvm_t* vm = c.vm;
		auto& ip = *(vm->curinstr);
		uint8_t
		    &out = (ip.blogic.bvr1_to_bvr2_flag ? vm->regs.bvr2 : vm->regs.bvr1),
		    &in = (ip.blogic.bvr1_to_bvr2_flag ? vm->regs.bvr1 : vm->regs.bvr2)
		;

		const uint8_t bmask = 0b00000001;

		switch(uvm_blogic_op(ip.blogic.logic_op)) {
			case uvm_blogic_op::_or: out |= in; break;
			case uvm_blogic_op::_and: out &= in; break;
			case uvm_blogic_op::_nor: out = ~(out | in); break;
			case uvm_blogic_op::_nand: out = ~(out & in); break;
			case uvm_blogic_op::_xor: out ^= in; break;
			default: return int(uvm_signal::sigill); //illegal instruction detected!
		}
		out &= bmask;
		return int(uvm_signal::sigok);
	}

	int _uvmopfunc_branch(uvmcalldata_t& c){
        uvm_t* vm = c.vm;
        uvmregs_t& regs = vm->regs;
        auto& ip = *(vm->curinstr);
        const bool conditional_value = (ip.branch.cond_is_bvr2_flag ? bool(regs.bvr2) : bool(regs.bvr1));
        if (conditional_value) {
            return int(uvm_signal::sigok);
        }
        else {
            uint16_t num = (ip.branch.iftrue_nisrs_lo & 0xFFU) | ((ip.branch.iftrue_nisrs_hi & 0xFFU) << 8);
            regs.ip.offset += num*4;
            return int(uvm_signal::sigjmp);
        }
    }
    int _uvmopfunc__goto(uvmcalldata_t& c) {
        auto* vm = c.vm;
        auto& ip = *(vm->curinstr);
        uint32_t dstidx = 0;
        dstidx |= (ip._goto.dstidx_lo & uint32_t(0xFF));
        dstidx |= ((ip._goto.dstidx_med & uint32_t(0xFF)) << 8);
        dstidx |= ((ip._goto.dstidx_hi & uint32_t(0xFF)) << 16);
        if (!vm->_reljmp(dstidx)){return int(uvm_signal::sigsegv);}
        return int(uvm_signal::sigjmp);
    }
    int _uvmopfunc_putchr(uvmcalldata_t& c) {
        auto* vm = c.vm;
        auto& ip = *(vm->curinstr);
        uvmregptr_t src;
        if (!uvm_get_reg_ptr(src, vm->regs, uint16_t(ip.putchr.chr_reg & 63U))){
            return int(uvm_signal::sigill);
        }

        uint16_t idx = ((ip.putchr.chr_idx_hi & 0xFFU) << 4) | (ip.putchr.chr_idx_lo & 0xFU);
        size_t ofs = 0;
        switch (ip.putchr.chr_order & 3U) {
            default: {
                ofs = idx;
                if (ofs >= src.datasize){
                    return int(uvm_signal::sigsegv);
                }
                std::cout << ((char*)src.dataptr)[ofs];
                std::cout.flush();
            }
        }
        return int(uvm_signal::sigok);
    }
    /*
    static const char _UVM_BINARY_FILE_IDENTIFIER[12] = {
    	'D', 'J', 'U', 'V', 'M', 'X', 'F', 'I', 'D', '\r', '\n', '\0'
    };
    enum struct uvm_section_id : uint16_t {
        null = 0,
        meta = 1,
        symt = 2,
        data = 3,
    	code = 4
    };
    struct UVMBinaryFile;
    struct UVMBinarySection {
        protected:
            UVMBinaryFile* _file = nullptr;
            uvm_section_id _id = uvm_section_id::null;
        public:
            virtual void read_data(std::istream& istr, const uint32_t len) {;}
            virtual void write_data(std::ostream& ostr) {;}

            virtual void on_create() {;}
            virtual void on_destroy() {;}

        public:
	    	const uvm_section_id& id = _id;
	    	UVMBinaryFile* const& file = _file;

	    	void write(std::ostream& ostr)
	    	{
	    		binio::pack<uvm_section_id, uint16_t>(ostr, &this->_id, 1, true); ostr.flush();
	    	    auto lenpos = ostr.tellp();
	    	    uint32_t len = 0;
	    	    ostr.write("\0\0\0\0", 4); ostr.flush();
	    	    auto datastartpos = ostr.tellp();
	    	    this->write_data(ostr); ostr.flush();
	    	    auto dataendpos = ostr.tellp();
	    	    len = uint32_t(size_t(dataendpos) - size_t(datastartpos));
	    	    ostr.seekp(lenpos);
	    	    binio::pack<uint32_t>(ostr, &len, 1, true); ostr.flush();
	    	    ostr.seekp(dataendpos);
	    	}
	    	void read(std::istream& istr)
	    	{
	    		;

	    	}
	    	UVMBinarySection(UVMBinaryFile& f, const uvm_section_id i) : _file(&f), _id(i) {}
	    	~UVMBinarySection();

    };

    struct UVMSymbolTableSection : public UVMBinarySection {
    	using UVMBinarySection::UVMBinarySection;
    };

    struct UVMBinaryFile {
    	std::vector<UVMBinarySection*> sections = {};

        template <class ST>
        ST* _newsect(const uvm_section_id id, const size_t insertat=std::string::npos)
        {
            auto& self = *this;
        	ST* sect = new ST(self, id);
        	if (insertat == std::string::npos)
        	{
        		self.sections.push_back(sect);
        	}
        	else
        	{
        		self.sections.insert(self.sections.begin() + insertat + 1, sect);
        	}
        	sect->on_create();
        	return sect;
        }
        UVMBinarySection* new_section(const uvm_section_id id, const size_t insertat=std::string::npos){
            #define _section_creator(_tn) (UVMBinarySection*)(_newsect<_tn>(id, insertat))
            switch (id)
            {
                case uvm_section_id::symt: return _section_creator(UVMSymbolTableSection);
            	default: return _section_creator(UVMBinarySection);
            }
            #undef _section_creator
        }

        void clear()
        {
        	while (this->sections.size() > 0)
        	{
        		delete this->sections.back();
        	}
        }



    };
    UVMBinarySection::~UVMBinarySection() {
	   	    this->on_destroy();
	   		bool _foundit = false;
	   		auto it = this->_file->sections.begin();
	   		for (; it != this->_file->sections.end(); it++)
	   		{
	   			if (*it == this){
	   				_foundit = true;
	   				break;
	   			}
	   		}
	   		if (_foundit) {
	   			this->_file->sections.erase(it);
	   		}
    }
    */
    struct uvm_assembler_output {
        std::unordered_map<std::string, std::vector<uint8_t>> data = {};
        std::unordered_map<std::string, uint32_t> symbols = {};
        std::vector<_uvminstr_t> code = {};
    };
    //int assembler1(std::vector<_uvminstr_t>& out, const std::string& in)
    int assembler1(uvm_assembler_output& asmout, const std::string& in)
    {
        auto& out = asmout.code;
    	std::stringstream inasm(in);
    	inasm >> std::skipws;
    	struct _scalar_type {
    		uint8_t cls, ord;
    	};

    	std::unordered_map<std::string, _scalar_type> _scalartypes = {};
    	const std::string iufp = "iufp";
    	for (int cls = 0; cls < 4; cls++) {
    		for (int ord = 0, ord2 = 1; ord <= 3; ord++, ord2 *= 2){
    			std::string stn(&(iufp[cls]), 1);
    			stn += std::to_string(ord2);
    			//std::cout << stn << '\n';
    			_scalartypes[stn] = _scalar_type{.cls=uint8_t(cls & 0b0111), .ord=uint8_t(ord & 0b011)};
    		}
    	}

    	/* LEGEND OF DATA TYPES:
    	 *
    	 *  TIP:
    	 *     the last char (the digit) of the 2 character type name
    	 *     denotes the scalar's size in bits (assuming 1 byte == 8 bits.)
    	 *
    	 *     eg:
    	 *
    	 *       1 = 8 bits = 1 byte
    	 *       2 = 16 bits = 2 bytes
    	 *       4 = 32 bits = 4 bytes
    	 *       8 = 64 bits = 8 bytes
    	 *
    	 *
    	 * i1 = int8_t
    	 * i2 = int16_t
    	 * i4 = int32_t
    	 * i8 = int64_t
    	 *
    	 * u1 = uint8_t
    	 * u2 = uint16_t
    	 * u4 = uint32_t
    	 * u8 = uint64_t
    	 *
    	 * f4 = 32-bit single precision IEEE-754 floating point number (a.k.a. float)
    	 * f8 = 64-bit double precision IEEE-754 floating point number (a.k.a. double)
    	*/

    	std::unordered_map<std::string, unsigned char> _varithops = {
    		{"vadd", '+'},
    		{"uvadd", '+'},
    		{"vsub", '-'},
    		{"uvsub", '-'},
    		{"vmul", '*'},
    		{"uvmul", '*'},
    		{"vdiv", '/'},
    		{"uvdiv", '/'}
    	};

    	std::unordered_map<std::string, uint8_t> _regmap = {
    	    {"vma", 0},
    	    {"vml", 1},
    	    {"vmr", 2},
    	    {"vrt", 3},

    	    {"ip",  4},

    	    {"gpr1", 5},
    	    {"gpr2", 6},
    	    {"gpr3", 7},
    	    {"gpr4", 8},

    	    {"ptr1", 9},
    	    {"ptr2", 10},

    	    {"bvr1", 11},
    	    {"bvr2", 12}
    	};

    	std::unordered_map<std::string, uvm_vcmp_subopcode> _vcmp_ops = {
			{"==", uvm_vcmp_subopcode::eq},
			{"!=", uvm_vcmp_subopcode::ne},
			{"<", uvm_vcmp_subopcode::lt},
			{">", uvm_vcmp_subopcode::gt},
			{"<=", uvm_vcmp_subopcode::le},
			{">=", uvm_vcmp_subopcode::ge},
			{"&&", uvm_vcmp_subopcode::truths_and},
			{"||", uvm_vcmp_subopcode::truths_or}
		};

    	std::unordered_map<std::string, uvm_blogic_op> _blogic_ops = {
			{"or", uvm_blogic_op::_or},
			{"and", uvm_blogic_op::_and},
			{"nor", uvm_blogic_op::_nor},
			{"nand", uvm_blogic_op::_nand},
			{"xor", uvm_blogic_op::_xor}
		};

        struct _branchstackent_t {
            size_t branch_isr_ofs;
        };

        std::vector<_branchstackent_t> branch_stack = {};

        std::unordered_map<std::string, uint32_t> labels = {};

        struct _unresolved_goto {
            size_t isridx;
            std::string labelname;
        };

        std::vector<_unresolved_goto> _gotos = {};

    	while (inasm)
    	{
            const size_t last_input_offset = inasm.tellg();
    		std::string op;
    	    inasm >> op;

    	    if (op == ""){break;}
    	    else if (op.size() >= 2 && (op[0] == '/' && op[1] == '/')) {
    	    	while (true) {
    	    		int c;
   	    			c = inasm.get();
   	    			if (c == EOF || c == '\r' || c == '\n'){break;}
    	    	}
    	    }
            else if (op.back() == ':'){
                std::string labelname = op;
                labelname.pop_back();
                if (labels.count(labelname) > 0){
                    std::stringstream errss;
                    errss << "redefinition of label " << std::quoted(labelname) << " at ofs=" << last_input_offset;
                    throw std::runtime_error(errss.str());
                }
                labels[labelname] = out.size();
            }
    	    else if (_varithops.count(op) > 0)
    	    {
    	        std::string sct; inasm >> sct;
    	        int numscalars = 1; inasm >> numscalars;

    	        _uvminstr_t newinstr = {.opcode=uvm_opcodes::varith};
    	    	newinstr.varith.arithop = _varithops[op];

    	    	newinstr.varith.scalar_class = _scalartypes[sct].cls & 0b0111U;
    	    	newinstr.varith.scalar_order = _scalartypes[sct].ord & 0b011U;
    	    	newinstr.varith.unary_operation = (op[0] == 'u');
    	    	newinstr.varith.r2l_operands = false;
    	    	newinstr.varith.numscalars = uint8_t((numscalars-1) & 0xF);

    	        out.push_back(newinstr);
    	    }
            else if (op == "goto") {
                std::string dstlabel;
                inasm >> dstlabel;
                _unresolved_goto _ent = {};
                _ent.isridx = out.size();
                _ent.labelname = dstlabel;
                _gotos.push_back(_ent);
                out.push_back(_uvminstr_t{.opcode=uvm_opcodes::_goto});
            }
            else if (op == "putchr8") {
                std::string srcreg;
                uint16_t srcidx;
                inasm >> srcreg >> srcidx;
                out.push_back(_uvminstr_t{.opcode=uvm_opcodes::putchr});
                auto& isr = out.back();
                isr.putchr.chr_order = 1U;
                isr.putchr.chr_reg = _regmap.at(srcreg) & 63U;
                isr.putchr.chr_idx_lo = (srcidx & 0xFU);
                isr.putchr.chr_idx_hi = (srcidx >> 4) & 0xFFU;
            }
            else if (op == "branch1" || op == "branch2") {
                const bool uses_bvr2 = (op == "branch2");
                _uvminstr_t newinstr = {.opcode=uvm_opcodes::branch};
                newinstr.branch.cond_is_bvr2_flag = uses_bvr2;
                newinstr.branch.iftrue_nisrs_hi = 0;
                newinstr.branch.iftrue_nisrs_lo = 0;
                _branchstackent_t nubse = {};
                nubse.branch_isr_ofs = out.size();
                branch_stack.push_back(nubse);
                out.push_back(newinstr);
            }
            else if (op == "@endbranch") {
                if (branch_stack.size() == 0){
                    throw std::runtime_error("bad @endbranch at ofs=" + std::to_string(last_input_offset) + " (branch stack is currently empty)");
                }
                const _branchstackent_t bse = branch_stack.back();
                branch_stack.pop_back();
                _uvminstr_t& branchisr = out[bse.branch_isr_ofs];
                size_t delta = (out.size() - bse.branch_isr_ofs);
                branchisr.branch.iftrue_nisrs_lo = uint16_t(delta & 0xFFU);
                branchisr.branch.iftrue_nisrs_hi = uint16_t(delta & 0xFF00U) >> 8;
            }
    	    else if (op == "term")
    	    {
    	    	out.push_back(_uvminstr_t{.opcode=0});
    	    }
    	    else if (op == "regmov")
    	    {
    	    	std::string dstr, srcr;
    	    	uint16_t num = 0;
    	    	inasm >> dstr >> srcr >> num;

    	    	out.push_back(_uvminstr_t{
    	    		.opcode=uvm_opcodes::regmov,
    	    		.regmov={
    	    			.dst=uint8_t(_regmap.at(dstr) & 0b0111111U),
    	    			.src=uint8_t(_regmap.at(srcr) & 0b0111111U),
    	    			.val_lo=uint8_t(num & 0xFU),
    	    			.val_hi=uint8_t((num >> 8) & 0xFFU)
    	    		}
    	    	});
    	    }
    	    else if (op == "addr2ptr")
    	    {
    	    	std::string dstpn, srcvn, srcvt;
    	        unsigned int srcvi = 0;
    	    	inasm >> dstpn >> srcvn >> srcvt >> srcvi;

    	    	if (_scalartypes.count(srcvt) == 0 || (srcvt[0] != 'u' && srcvt[0] != 'i')){
                    throw std::runtime_error("addr2ptr: bad typename for in-address: " + srcvt);
    	    	}

    	    	uint8_t iord = _scalartypes[srcvt].ord;
    	    	iord &= 0b011U;

    	    	uint8_t dstp = _regmap.at(dstpn) & 63U;
    	    	uint8_t srcv = _regmap.at(srcvn) & 63U;

    	    	uint8_t srcvip = (srcvi & 0xFU);

    	    	out.push_back(_uvminstr_t{
    	    		.opcode=uvm_opcodes::ptrconv,
    	    		.ptrconv={
    	    			.addr2ptr={
    	    				.subopcode=uint8_t(0U),
    	    				.is_unsigned_integer=bool(srcvt[0] == 'u'),
    	    				.integer_order=iord,

    	    				.srcvr=srcv,
    	    				.dstpr=dstp,
    	    				.srcvrelem=srcvip
    	    			}
    	    		}
    	    	});
    	    }
    	    else if (op == "ptr2addr")
    	    {
    	    	out.push_back(_uvminstr_t{.opcode=uvm_opcodes::ptrconv});
    	    	auto& isr = out.back();
    	    	isr.ptrconv.ptr2addr.subopcode = 1;
    	    	std::string dstvn, srcpn, dstvt;
    	    	unsigned int dsti = 0;
    	    	inasm >> dstvt >> dstvn >> dsti >> srcpn;
    	    	if (_scalartypes.count(dstvt) == 0 || (dstvt[0] != 'u' && dstvt[0] != 'i')){
                    throw std::runtime_error("ptr2addr: bad typename for out-address: " + dstvt);
    	    	}

    	    	isr.ptrconv.ptr2addr.is_unsigned_integer = (dstvt[0] == 'u');
    	    	isr.ptrconv.ptr2addr.integer_order = uint8_t(_scalartypes[dstvt].ord) & uint8_t(0b11U);
    	    	isr.ptrconv.ptr2addr.srcpr = uint8_t(_regmap.at(srcpn) & 63U);
    	    	isr.ptrconv.ptr2addr.dstvr = uint8_t(_regmap.at(dstvn) & 63U);
    	    	isr.ptrconv.ptr2addr.dstvrelem = uint8_t(dsti & 0xFU);

    	    }
    	    else if (op == "getregptr")
    	    {
    	        _uvminstr_t isr = {.opcode=uvm_opcodes::ptrconv};
    	        isr.ptrconv.getregptr.subopcode = 2;
    	        std::string dstpn, srcrn;
    	        inasm >> dstpn >> srcrn;

    	        isr.ptrconv.getregptr.dstptr_r = uint8_t(_regmap.at(dstpn) & 63U);
    	        isr.ptrconv.getregptr.regno = uint8_t(_regmap.at(srcrn) & 63U);

    	        out.push_back(isr);
    	    }
    	    else if (op == "vrtinc")
    	    {
    	    	std::string stn;
    	    	unsigned int elemidx = 0;
    	    	int base = 0, exponent = 0;
    	    	inasm >> stn >> elemidx >> base >> exponent;

    	    	out.push_back(_uvminstr_t{
    	    		.opcode=uvm_opcodes::vrtinc,
    	    		.vrtinc={
    	    			.value_class=uint8_t(_scalartypes[stn].cls & 0b0111U),
    	    			.value_order=uint8_t(_scalartypes[stn].ord & 0b011U),
    	    			.elemidx=uint8_t(elemidx & 0xFU),

    	    			.base=int8_t(base & 0b01111111),
    	    			.exponent=int8_t(exponent & 0xFF)
    	    		}
    	    	});
    	    }
    	    else if (op == "vrtinc_il" || op == "vrtinc_chr")
    	    {
    	    	std::string stn;
    	    	unsigned int elemidx = 0;
    	    	int32_t il = 0;
    	    	inasm >> stn >> elemidx;
    	    	if (op == "vrtinc_chr")
    	    	{
    	    	    inasm.get();
    	    		il = inasm.get();
    	    	}
    	    	else
    	    	{
    	    		inasm >> il;
    	    	}
    	    	_uvminstr_t isrb = {.opcode=uvm_opcodes::vrtinc};
    	    	isrb.vrtinc.value_class = uint8_t(_scalartypes[stn].cls & 0b0111U);
    	    	isrb.vrtinc.value_order = uint8_t(_scalartypes[stn].ord & 0b011U);
    	    	isrb.vrtinc.elemidx = uint8_t(elemidx & 0xFU);

    	    	for (int32_t i = 0; i < 32; i++)
    	    	{
    	    		if (((il >> i) & 0b1))
    	    		{
    	    			auto isr = isrb;
    	    			isr.vrtinc.base = (il >= 0 ? 2 : -2);
    	    			isr.vrtinc.exponent = int8_t(i & 0xFF);
    	    			out.push_back(isr);
    	    		}
    	    	}
    	    }
    	    else if (op == "hostcall")
    	    {
    	    	uint16_t num = 0;
    	    	inasm >> num;
    	    	_uvminstr_t isr = {.opcode=uvm_opcodes::hostcall};
    	    	isr.hostcall.hcallno.lo = uint8_t(num & 0xFFU);
    	    	isr.hostcall.hcallno.hi = uint8_t((num >> 8) & 0xFFU);
    	    	out.push_back(isr);
    	    }
    	    else if (op == "mov" || op == "movr")
    	    {
    	        std::string dstptr_r, srcptr_r, num_u4_r;
    	        unsigned int num_u4_idx = 0;

    	        inasm >> dstptr_r >> srcptr_r >> num_u4_r >> num_u4_idx;

    	        _uvminstr_t isr = {.opcode=uvm_opcodes::mov};
    	        isr.mov.reverse_copy_flag = (op == "movr");
    	        isr.mov.dstptr_r = uint8_t(_regmap.at(dstptr_r) & 63U);
    	        isr.mov.srcptr_r = uint8_t(_regmap.at(srcptr_r) & 63U);
    	        isr.mov.num_u4_r = uint8_t(_regmap.at(num_u4_r) & 63U);
    	        isr.mov.num_u4_idx = uint8_t((num_u4_idx) & 0xFU);
    	        out.push_back(isr);
    	    }
    	    else if (op == "fill")
    	    {
    	    	std::string dstptr_r, fillbyte_r, numbytes_r;
    	    	inasm >> dstptr_r >> fillbyte_r >> numbytes_r;

    	    	_uvminstr_t isr = {.opcode=uvm_opcodes::fill};
    	    	isr.fill.header.subopcode = 0;
    	    	isr.fill.fill.dstptr_r = uint8_t(_regmap.at(dstptr_r) & 63U);
    	    	isr.fill.fill.fillbyte_r = uint8_t(_regmap.at(fillbyte_r) & 63U);
    	    	isr.fill.fill.numbytes_r = uint8_t(_regmap.at(numbytes_r) & 63U);
    	    	out.push_back(isr);
    	    }
    	    else if (op == "vcmp" || op == "vcmp1" || op == "vcmp2") {
				bool res_in_bvr2 = (op == "vcmp2");
				std::string vtn, op;
				size_t lidx = 0, ridx = 0;
				inasm >> vtn >> lidx >> op >> ridx;
				_uvminstr_t isr = {.opcode=uvm_opcodes::vcmp};
				isr.vcmp.put_result_in_bvr2_flag = res_in_bvr2;
				isr.vcmp.subopcode = uint8_t(_vcmp_ops.at(op));
				_scalar_type _st = _scalartypes.at(vtn);
				isr.vcmp.value_class = uint8_t(_st.cls & 0b0111U);
				isr.vcmp.value_order = uint8_t(_st.ord & 0b011U);
				isr.vcmp.vma_idx = uint8_t(lidx & 0xFU);
				isr.vcmp.vmr_idx = uint8_t(ridx & 0xFU);
				out.push_back(isr);
			}
			else if (op == "blogic" || op == "rblogic") {
				_uvminstr_t isr = {.opcode=uvm_opcodes::blogic};
				isr.blogic.bvr1_to_bvr2_flag = (op[0] == 'r');
				std::string subopn;
				inasm >> subopn;
				isr.blogic.logic_op = uint8_t(_blogic_ops.at(subopn));
				out.push_back(isr);
			}
    	    else {
	       		throw std::runtime_error("unknown instruction name at ofs="+std::to_string(last_input_offset)+": "+op);
	       	}
    	}

        for (auto& _goto : _gotos) {
            if (labels.count(_goto.labelname) == 0){
                std::stringstream errss;
                errss << "goto with undefined dst label " << std::quoted(_goto.labelname);
                throw std::runtime_error(errss.str());
            }
            uint32_t jmpdst = labels[_goto.labelname];
            auto& isr = out[_goto.isridx];
            isr._goto.dstidx_lo = jmpdst & uint32_t(0xFF);
            isr._goto.dstidx_med = (jmpdst >> 8) & uint32_t(0xFF);
            isr._goto.dstidx_hi = (jmpdst >> 16) & uint32_t(0xFF);
        }

        for (auto& branchent : branch_stack) {
            std::cout << "warning: unterminated branch starting from ofs=" << branchent.branch_isr_ofs << ".\n";
        }

    	return 0;
    }

}};
#endif

#if (defined(DJUTIL_NEEDS_imaging) && !defined(__DJUTIL_H_imaging_LOADED))
#define __DJUTIL_H_imaging_LOADED
#include <cmath>
#include <valarray>
#include <array>
#include <type_traits>
#define _DJUTIL_H_NAMESPACE_USING_GNUVECTORS
namespace djutil {namespace imaging {

	enum class color_format : unsigned int {
		grayscale = 1,
		grayalpha = 2,
		rgb       = 3,
		rgba      = 4
	};

	enum class pixel_type : unsigned int {
		u8  = 0,
		u16 = 1,
		f32 = 2
	};

	const size_t _PXTSIZES[] = {
		1,
		2,
		4,
	};

	using fpscalar = float;
	inline fpscalar _clampfp(const fpscalar v, const fpscalar minv=0, const fpscalar maxv=1)
	{
		return (v < maxv ? (v > minv ? v : minv) : maxv);
	}
	template <class UIT=uint8_t>
	inline UIT _fp2ui(const fpscalar fp)
	{
		const UIT lo = 0, hi = ~lo;
		return (fp < 1 && fp > 0 ? UIT(round(fp * hi)) : (fp == 0 ? lo : hi));
	}

	struct imgdims_t {
		int w, h, c;
	};
	template <class CT>
	CT* vflip_ip(CT* pixels, const int width, const int height, const int cpp=1)
	{
	    const int hh = height/2;
	    const size_t rowlen = width*cpp, imglen = height*rowlen;

	    for (int y1 = 0, y2 = height-1; y1 < hh && y2 >= 0; y1++, y2--)
	    {
	    	CT *row1 = pixels+(y1*rowlen), *row2 = pixels+(y2*rowlen);
	    	std::swap_ranges(row1, row1+rowlen, row2);
	    }
		return pixels;
	}
	template <class CT>
	CT* vflip_ip(CT* pixels, const imgdims_t& dims) {
		return vflip_ip<CT>(pixels, dims.w, dims.h, dims.c);
	}

	template <class CT>
	CT* uflip_ip(CT* pixels, const int width, const int height, const int cpp=1)
	{
		const size_t rowlen = width*cpp, imglen = height*rowlen;
		CT *imgend = pixels + imglen, *px1 = nullptr;
		for (CT* row = pixels; row < imgend; row += rowlen){
		    for (int x1 = 0, x2 = width-1; x1 < (width/2) && x2 >= 0; x1++, x2--){
		        px1 = row+(x1*cpp);
		    	std::swap_ranges(px1, px1+cpp, row+(x2*cpp));
		    }
		}
		return pixels;
	}
	template <class CT>
	CT* uflip_ip(CT* pixels, const imgdims_t& dims) {
		return uflip_ip<CT>(pixels, dims.w, dims.h, dims.c);
	}

	template <class ICT, class FLT=float>
	ICT* ugamma_ip(ICT* pixels, const imgdims_t& dims, const FLT gamma, const int chanmask=0b1111) {
		const FLT fimax(~ICT(0));
		const FLT invfimax = 1/fimax;
		const FLT gam = 1/gamma;

		const bool _mask[4] = {
			bool(chanmask & 0b1000),
			bool(chanmask & 0b0100),
			bool(chanmask & 0b0010),
			bool(chanmask & 0b0001)
		};
	    //FLT _fchan[4] = {};

	    const int npx = dims.w * dims.h;
	    ICT* px = pixels;
	    for (int i = 0; i < npx; i++, px += dims.c)
	    {
	    	for (int c = 0; c < 4 && c < dims.c; c++)
	    	{
	    	    if (!_mask[c]){continue;}
	    		FLT cf = std::max(std::min(std::pow(px[c] * invfimax, gam), FLT(1)), FLT(0)) * fimax;
	    		px[c] = ICT(long(cf));
	    	}
	    }
	    return pixels;
	}

    template <class T>
    T* scaled_copy(T* dst, const T* src, const size_t dw, const size_t dh, const size_t sw, const size_t sh, const size_t c)
    {
        if (sw == dw && sh == dh){std::copy_n(src, dw*dh*c, dst); return dst;}
        const float
            xi = fpscalar(sw-1)/(dw-1),
            yi = fpscalar(sh-1)/(dh-1)
        ;
        for (size_t dy = 0; dy < dh; dy++)
        {
            const size_t sy(roundf(dy*yi));
            for (size_t dx = 0; dx < dw; dx++)
            {
                const size_t sx(roundf(dx*xi));
                std::copy_n(src + (((sy * sw) + sx) * c), c, dst + (((dy * dw) + dx) * c));
            }
        }
        return dst;
    }

    template <class T, const size_t SF=1024>
     // T = image pixel channel base type e.g. uint8_t for 8bpc images
     // SF = fixed point scaling factor.
    T* scaled_copy_fixed(T* dst, const T* src, const size_t dw, const size_t dh, const size_t sw, const size_t sh, const size_t c) {
        //nearest neighbor image scaling using only fixed point arithmetic!
        if (sw == dw && sh == dh){std::copy_n(src, dw*dh*c, dst); return dst;}
        const size_t
            xi = (SF*(sw-1))/(dw-1),
            yi = (SF*(sh-1))/(dh-1)
        ;
        //now do the actual scale using ONLY fixed point math
        for (size_t dy=0,dx; dy < dh; dy++) {
            const size_t my = yi*dy;
            size_t sy = my/SF;
            if (my-(sy*SF) >= (SF/2) && sy < (sh-1)){sy += 1;}
            for (dx=0; dx < dw; dx++) {
                const size_t mx = xi*dx;
                size_t sx = mx/SF;
                if (mx-(sx*SF) >= (SF/2) && sx < (sw-1)){sx += 1;}
                std::copy_n(src + (((sy*sw)+sx)*c), c, dst + (((dy*dw)+dx)*c));
            }
        }
        return dst;
    }
	/*
	typedef float _v4sf_t __attribute__((mode(V4SF)));
	typedef long _v4sl_t __attribute__((mode(V4SL)));
	union _imv4data_t {_v4sf_t v; float f[4];};
	template <typename F>
	struct alignas(float) imvec4_t {
		public:
		    union {
				_imv4data_t data;
				struct {float x, y, z, w;};
				struct {float r, g, b, a;};
				struct {float s, t;};
				struct {float u, v;};
			};
		    #define _imv4_binop_gen(_op) \
		        inline imvec4_t operator ## _op (const imvec4_t& o) const {return imvec4_t(this->data.v _op o.data.v);} \
		    \

		    _imv4_binop_gen(+)
		    _imv4_binop_gen(-)
		    _imv4_binop_gen(*)
		    _imv4_binop_gen(/)

		    inline imvec4_t operator%(const imvec4_t& o) const {
				_imv4data_t _nd = {}; _nd.v = this->v / o.v;
				_nd.f[0] = floorf(_nd.f[0]);
				_nd.f[1] = floorf(_nd.f[1]);
				_nd.f[2] = floorf(_nd.f[2]);
				_nd.f[3] = floorf(_nd.f[3]);
				return imvec4_t(_nd.v * o.v + (this->v % o.v));
			}

		    #undef _imv4_binop_gen
		    inline operator _imv4data_t() const {return this->data;}
		    inline imvec4_t& operator=(const imvec4_t& o) {this->data = o.data; return *this;}

		    inline const float& operator[](const int idx) const {return this->data.f[idx];}
		    inline float& operator[](const int idx) {return this->data.f[idx];}

		    inline imvec4_t operator-() const {return imvec4_t(-(this->data.v));}

		    inline float hsum() const {return (this->x + this->y + this->z + this->w);}

		    inline float dot(const imvec4_t& other) const {
				imvec4_t
			}

		    imvec4_t() : data.f{0.0f,0.0f,0.0f,0.0f} {}
		    imvec4_t(const imvec4_t& _imv4) : data(_imv4.data) {}
		    imvec4_t(const _v4sf_t& _v4) : data.v(_v4) {}
		    imvec4_t(const float& _u, const float& _v) : data.f{_u, _v} {}
		    imvec4_t(
		        const float& _x,
		        const float& _y,
		        const float& _z,
		        const float& _w
		    ) : data.f{_x,_y,_z,_w} {}
	};
	*/

	typedef float frgba_t __attribute__((vector_size(sizeof(float[4]))));
	typedef float _sfvec2_t __attribute__((vector_size(sizeof(float[2]))));
	static const int _TO_RGBA_IDXS[5][4] = {
		{-1,-1,-1,-1},
		{0,0,0,-1},
		{0,0,0,1},
		{0,1,2,-1},
		{0,1,2,3}
	};

	class RasterImage {
		public:
		    using pxetype = pixel_type;
		    using colorformat = color_format;
		    std::valarray<frgba_t> _palette = {}, _fpixels = {};
		    std::valarray<uint16_t> _ipixels = {};
		    inline size_t _xy2idx(const long x, const long y) const {
				size_t _x = x, _y = y;
				if (_x >= this->_width || _y >= this->_height){return (size_t)-1;}
				return (_y * this->_width) + _x;
			}

		private:
		    bool _valid = false;
		    size_t _width = 0, _height = 0, _npalents = 0, _npixels = 0;

		    bool _indexed = false;

		    bool _reset_image() {
				if (!this->_valid){return false;}
				this->_npalents = 0;
				this->_npixels = 0;
				this->_indexed = false;
				this->_palette.resize(0);
				this->_fpixels.resize(0);
				this->_ipixels.resize(0);
				this->_width = this->_height = 0;
				this->_valid = false;
				return true;
			}
			static void _cvt2f(float* out_floats, const pxetype in_et, const void* inarr, const size_t num=1) {
				if (out_floats == nullptr || num == 0){return;}
				if (inarr == nullptr) {std::fill_n(out_floats, num, 0.0f); return;}
				switch (in_et) {
					case pxetype::u8: {
						const auto* _inarr = (const uint8_t*)inarr;
						std::transform(_inarr, _inarr + num, out_floats, [](uint8_t _v)->float{return _v/255.0f;});
						break;
					}
					case pxetype::u16: {
						const auto* _inarr = (const uint16_t*)inarr;
						std::transform(_inarr, _inarr + num, out_floats, [](uint16_t _v)->float{return _v/65535.0f;});
						break;
					}
					default: {
						std::copy_n((const float*)inarr, num, out_floats);
					}
				}
			}
			void _init_truecolor(const size_t nw, const size_t nh, const colorformat nc, const pxetype npt, const void* ipixels=nullptr) {
				const auto cpp = size_t(nc);
				const auto pxesz = size_t(_PXTSIZES[int(npt)]);
				const auto pxsz = cpp * pxesz;
				this->_reset_image();
				this->_width = nw;
				this->_height = nh;
				this->_fpixels.resize(nw * nh);

				float _tof[4];
				const size_t npxs = nw * nh;
				if (ipixels != nullptr) {
					for (size_t i = 0; i < npxs; i++) {
						frgba_t& curpx = this->_fpixels[i];
						const void* pxptr(((const char*)ipixels)+(i*pxsz));
						RasterImage::_cvt2f(_tof, npt, pxptr, cpp);

						for (size_t j = 0; j < 4; j++) {
							const int& _idx = _TO_RGBA_IDXS[int(nc)][j];
							if (_idx == -1) {curpx[j] = 1.0f;}
							else if (_idx >= 0) {curpx[j] = _tof[_idx];}
							else {}
						}
					}
				}
				this->_valid = true;
			}
		    bool _ensure_truecolor() {
				if (!this->_valid || !this->_indexed){return false;}
				this->_indexed = false;
				this->_fpixels.resize(this->_width * this->_height);
				for (size_t i = 0; i < this->_ipixels.size(); i++)
				{
					this->_fpixels[i] = this->_palette[this->_ipixels[i]];
				}
				this->_ipixels.resize(0);
				this->_palette.resize(0);
				return true;
			}
		public:
		    const size_t &width = _width, &height = _height;
		    RasterImage* init_truecolor(const size_t nw, const size_t nh, const colorformat nc, const pxetype npt, const void* ipixels=nullptr) {
				this->_init_truecolor(nw,nh,nc,npt,ipixels);
				return this;
			}
			RasterImage* make_truecolor() {
				this->_ensure_truecolor();
				return this;
			}
		    frgba_t getpixel(const long x, const long y) const {
	            if (x < 0 || y < 0 || x >= this->_width || y >= this->_height){return frgba_t{};}
	            else if (this->_indexed) {
					return this->_palette[this->_ipixels[this->_xy2idx(x,y)]];
				}
				else {
					return this->_fpixels[this->_xy2idx(x,y)];
			    }
			}
			void setpixel(const long x, const long y, const frgba_t& nv) {
	            if (x < 0 || y < 0 || x >= this->_width || y >= this->_height){return;}
	            else if (this->_indexed) {
					return;
				}
				else {
					this->_fpixels[this->_xy2idx(x,y)] = nv;
			    }
			}
		    inline size_t num_pixels() const {return this->_width * this->_height;}

		    size_t to_rgba32(uint8_t* output) const {
				uint8_t* _out = output;
			    size_t nb = 0;
			    for (size_t y = 0; y < this->_height; y++) {
					for (size_t x = 0; x < this->_width; x++, nb += 4, _out += 4) {
					    const frgba_t px = this->getpixel(x,y);
					    for (size_t c = 0; c < 4; c++) {
							_out[c] = (uint8_t)(roundf(std::min(std::max(px[c], 0.0f), 1.0f) * 255.0f));
						}
				    }
				}
				return nb;
			}
			size_t to_rgba64(uint16_t* output) const {
				uint16_t* _out = output;
			    size_t nb = 0;
			    for (size_t y = 0; y < this->_height; y++) {
					for (size_t x = 0; x < this->_width; x++, nb += 8, _out += 4) {
					    const frgba_t px = this->getpixel(x,y);
					    for (size_t c = 0; c < 4; c++) {
							_out[c] = (uint16_t)(roundf(std::min(std::max(px[c], 0.0f), 1.0f) * 65535.0f));
						}
				    }
				}
				return nb;
			}
		    #ifdef DJUTIL_IMAGING_USE_STBI_LOAD

		    RasterImage* load_stbi(std::istream& infp) {
				int w, h, c;
				stbi_io_callbacks _cbs = {
					.read=[](void* hnd, char* data, int size)->int{ return ((std::istream*)hnd)->read(data, size).gcount(); },
					.skip=[](void* hnd, int n){((std::istream*)hnd)->seekg(n, std::ios::cur);},
					.eof=[](void* hnd) -> int {return ((std::istream*)hnd)->eof();}
			    };
				uint16_t* px = stbi_load_16_from_callbacks(&_cbs, (void*)&infp, &w, &h, &c, 4);
				if (px == nullptr) {
					return this;
				}
				this->init_truecolor(w, h, colorformat::rgba, pxetype::u16, px);
				//std::copy_n((frgba_t*)px, this->num_pixels(), &(this->_fpixels[0]));

				stbi_image_free(px); px = nullptr;
				return this;
			}

		    #endif

		    #ifdef DJUTIL_IMAGING_USE_STBI_WRITE

		    enum struct outfiletype : int {
				png = 0,
				tga = 1,
				jpg = 2
			};

		    RasterImage* write_stbi(std::ostream& outfp, const outfiletype oft) {
				if (!this->_valid){return this;}

				std::valarray<uint8_t> u8px; u8px.resize(this->num_pixels() * 4);
				this->to_rgba32(&u8px[0]);

				stbi_write_func* wf = [](void* ctx, void* data, int size){
					((std::ostream*)ctx)->write((char*)data, size);
				};

				switch (oft) {
					case outfiletype::png:
					    stbi_write_png_to_func(wf, (void*)&outfp, this->_width, this->_height, 4, &u8px[0], 0);
					    break;
					case outfiletype::tga:
					    stbi_write_tga_to_func(wf, (void*)&outfp, this->_width, this->_height, 4, &u8px[0]);
					    break;
					case outfiletype::jpg:
					    stbi_write_jpg_to_func(wf, (void*)&outfp, this->_width, this->_height, 4, &u8px[0], 0);
					    break;
					default: ;
				}
				return this;
			}

		    #endif
		    RasterImage* load_rim(std::istream& rimf) {
				char _ident[7] = {};
				binio::unpack(_ident, rimf, 7);
				if (!memcmp(_ident, "RAWIMG\0", 7)) {
					uint32_t dims[3];
					binio::unpack(dims, rimf, 3, true);
					//std::string _rpixels; _rpixels.resize(dims[0]*dims[1]*dims[2]);
					//binio::unpack(&_rpixels.front(), rimf, _rpixels.size());
					this->init_truecolor(dims[0], dims[1], colorformat(dims[2]), pxetype::u8, nullptr);
					for (frgba_t& px : this->_fpixels) {
						uint8_t _pxrd[4] = {};
						rimf.read((char*)_pxrd, dims[2]);
						for (size_t j = 0; j < 4; j++) {
							int idx = _TO_RGBA_IDXS[dims[2]][j];
							px[j] = (idx < 0 ? 1.0f : _pxrd[idx]/255.0f);
						}
					}
				}
				else if (!memcmp(_ident, "RAWIMGP", 7)) {
					uint32_t dims[3];
					binio::unpack(dims, rimf, 3, true);
					this->init_truecolor(dims[0], dims[1], colorformat(dims[2]), pxetype::u8, nullptr);
					frgba_t _pal[256] = {};
					const int* _torgba_idxs = _TO_RGBA_IDXS[dims[2]];
					for (size_t i = 0; i < 256; i++) {
						uint8_t _palent[4] = {};
						binio::unpack(_palent, rimf, dims[2]);
						for (size_t j = 0; j < 4; j++) {
							int idx = _torgba_idxs[j];
							if (idx < 0){_pal[i][j] = 1.0f;}
							else {
								_pal[i][j] = _palent[j]/255.0f;
							}
						}
					}

					for (frgba_t& px : this->_fpixels) {
						px = _pal[rimf.get()];
					}
				}
				else {
					throw std::runtime_error("Not a rim!");
				}
				return this;
			}
		    RasterImage* write_rim(std::ostream& ostr) {
				if (!this->_valid){return this;}
				else if (this->_indexed) {
					ostr.write("RAWIMGP", 7);
					uint32_t dims[3] = {uint32_t(this->_width), uint32_t(this->_height), uint32_t(4)};
					binio::pack(ostr, dims, 3, true);
					for (size_t i = 0; i < 256; i++) {
						frgba_t palent = {};
						if (i < this->_palette.size()) {
							palent = this->_palette[i];
						}
						for (size_t j = 0; j < 4; j++) {
							ostr.put(int(roundf(255.0f * std::max(std::min(palent[j], 1.0f), 0.0f))));
						}
					}

					for (auto& idx : this->_ipixels) {
						ostr.put(int(std::max(std::min(idx, uint16_t(255)), uint16_t(0))));
					}
				}
				else {
					ostr.write("RAWIMG\0", 7);
					uint32_t dims[3] = {uint32_t(this->_width), uint32_t(this->_height), uint32_t(4)};
					binio::pack(ostr, dims, 3, true);
					std::valarray<uint8_t> _px; _px.resize(this->num_pixels() * 4);
					this->to_rgba32(&_px[0]);
					ostr.write((char*)(&_px[0]), _px.size());
				}
				return this;
			}

		    RasterImage* vflip() {
				if (!this->_valid){return this;}
				else if (this->_indexed) {
					vflip_ip(&(this->_ipixels[0]), this->_width, this->_height, 1);
				}
				else {
					vflip_ip(&(this->_fpixels[0]), this->_width, this->_height, 1);
				}
				return this;
			}
		    RasterImage* uflip() {
				if (!this->_valid){return this;}
				else if (this->_indexed) {
					uflip_ip(&(this->_ipixels[0]), this->_width, this->_height, 1);
				}
				else {
					uflip_ip(&(this->_fpixels[0]), this->_width, this->_height, 1);
				}
				return this;
			}

			RasterImage* draw_solid_circle(const float x, const float y, const float radius, const frgba_t fill) {
				_sfvec2_t origin = {x, y}, bmin = {x-radius, y-radius}, bmax = {x+radius, y+radius}, cur = {};
				for (cur[1] = bmin[1]; cur[1] <= bmax[1]; cur[1]++) {
					for (cur[0] = bmin[0]; cur[0] <= bmax[0]; cur[0]++) {
						_sfvec2_t delta = cur - origin, delta2 = delta*delta;
						float distance = sqrtf(delta2[0] + delta2[1]);
						if (distance <= radius){this->setpixel(long(roundf(cur[0])), long(roundf(cur[1])), fill);}
					}
				}
				return this;
			}

		    RasterImage() {}
		    RasterImage(const size_t& w, const size_t& h) {this->_init_truecolor(w,h,colorformat::rgba,pxetype::f32,nullptr);}
		    RasterImage(const RasterImage& other) :
		        _valid(other._valid),
		        _width(other._width),
		        _height(other._height),
		        _palette(other._palette),
		        _ipixels(other._ipixels),
		        _fpixels(other._fpixels),
		        _indexed(other._indexed)
		    {}
		    ~RasterImage() = default;

	};

	template <class SHD, const size_t NIMGS=1>
	void draw_shader(RasterImage* images[NIMGS], SHD& shader) {
		long xmin=0, ymin=0, xmax=0, ymax=0;
		shader.calc_drawbounds(xmin, ymin, xmax, ymax);
		for (long y = ymin; y <= ymax; y++) {
			for (long x = xmin; x <= xmax; x++) {
				frgba_t _frags[NIMGS] = {};
				size_t nfr = shader.fragment(NIMGS, _frags, x, y);
				for (size_t i = 0; i < nfr; i++){images[i]->setpixel(x,y,_frags[i]);}
			}
		}
	}

}};
#endif

#if (defined(DJUTIL_NEEDS_dmd) && !defined(__DJUTIL_H_dmd_LOADED))
#define __DJUTIL_H_dmd_LOADED
namespace djutil {namespace dmd {
		namespace _io {
		    const bool hostLE = binio::_hostIsLE;
		    template <class VT, class RT=VT>
		    VT* unpack(VT* dst, std::istream& src, const size_t num=1, const bool le=hostLE)
		    {
		    	binio::unpack<VT, RT>(dst, src, num, le);
		    	return dst + num;
		    }
		    template <class WT, class VT=WT>
		    const VT* pack(std::ostream& dst, const VT* src, const size_t num=1, const bool le=hostLE)
		    {
		    	binio::pack<VT, WT>(dst, src, num, le);
		    	return src + num;
		    }
		};

		template <class T, size_t N>
		//#pragma pack(1)
		struct alignas(T) _array1dTN {
		    using elem_t = T;
		    private:
		        elem_t _elements[N] = {};
		    public:
		        size_t size() const {return N;}

		        operator elem_t*() {return this->_elements;}
		        operator const elem_t*() const {return this->_elements;}

		        elem_t& operator[](const ptrdiff_t i) const {return this->_elements[i];}

		        elem_t* operator+(const ptrdiff_t offs) {return this->_elements + offs;}
		        const elem_t* operator+(const ptrdiff_t offs) const {return this->_elements + offs;}

		        elem_t* operator-(const ptrdiff_t offs) {return this->_elements - offs;}
		        const elem_t* operator-(const ptrdiff_t offs) const {return this->_elements - offs;}

	            elem_t* begin() {return this[0] + 0;}
	            const elem_t* cbegin() const {return this[0] + 0;}

	            elem_t* end() {return this[0] + N;}
		        const elem_t* cend() const {return this[0] + N;}

		        template <class U=elem_t, size_t O=N>
		        bool operator==(const _array1dTN<U, O>& other) const
		        {
		        	if (N != O){return false;}
		        	return std::equal(this->cbegin(), this->cend(), other.cbegin());
		        }
		        template <class U=elem_t, size_t O=N>
		        bool operator!=(const _array1dTN<U, O>& other) const
		        {
		        	if (N != O){return true;}
		        	return !(std::equal(this->cbegin(), this->cend(), other.cbegin()));
		        }

		        friend std::ostream& operator<<(std::ostream& os, const _array1dTN<T,N>& self)
		        {
		            for (const T* v = self.cbegin(); v != self.cend(); v++){os << *v;}
		            return os;
		        }

		        _array1dTN() {}

		        template <class U=elem_t, size_t O=N>
		        _array1dTN(const _array1dTN<U, O>& o, const U defaultfill=U())
		        {
		            std::fill_n(this[0].begin(), N, defaultfill);
		            std::copy_n(o.cbegin(), std::min(N, O), this[0].begin());
		        }
		        template <class U=elem_t>
		        _array1dTN(const std::initializer_list<U>& il)
		        {
		            std::fill_n(this->_elements, N, elem_t());
		        	std::copy_n(il.begin(), std::min(N, il.size()), this->begin());
		        }

		        /*
		        template<class U=elem_t, size_t O=N>
		        _array1dTN(const U a[O], const elem_t defaultfill=elem_t())
		        {
		            std::cout << "array constructor\n";
		        	std::fill_n(this->_elements, N, defaultfill);
		        	std::copy(a, a + size_t(O <= N ? O : N), this->_elements);
		        }
		        */
		        _array1dTN(const elem_t* a, const size_t n, const elem_t defaultfill=elem_t())
		        {
		        	//std::cout << "array ctor\n";
		        	std::fill_n(this->_elements, N, defaultfill);
		        	std::copy_n(a, (n <= N ? n : N), this->_elements);
		        }

		        _array1dTN(const elem_t* a, const elem_t defaultfill=elem_t())
		        {
		        	//std::cout << "C-string ctor\n";
		        	std::fill_n(this->_elements, N, defaultfill);
		        	const elem_t term = elem_t();
		        	for (size_t i = 0; i < N && a[i] != term; i++)
		        	{
		        		this->_elements[i] = a[i];
		        	}
		        }

		};
		//#pragma pack()

		const _array1dTN<char, 8> _DMD_FILE_MAGIC = "DJMODEL\0";
		/*
		struct dmdnodetype {
		    char _ntypestr[4] = {' ', ' ', ' ', ' '};
		    dmdnodetype() {};
		    dmdnodetype(const char c0, const char c1=' ', const char c2=' ', const char c3=' ') : _ntypestr{c0, c1, c2, c3} {}
		    dmdnodetype(const dmdnodetype& other) {memcpy(this->_ntypestr, other._ntypestr, 4);}
		    inline bool operator==(const dmdnodetype& other) const
		    {
		    	return (memcmp(this->_ntypestr, other._ntypestr, 4) == 0);
		    }
		};
		*/
		using ddfkey_t = _array1dTN<char, 8>;

		const ddfkey_t _DMD_DDF_TERMKEY = "TERMKEY";
		const ddfkey_t _DMD_DDF_VERKEY = "DDFVER";

		struct _ddfwriter_t {
		    private:
		        std::ostream& ostr;
		        const ddfkey_t verkey, termkey;
		        bool _writing = true, _inkey = false;
		        ddfkey_t _curkey;
		        std::streampos _ddf_startpos, _ddf_kdstartpos;
		        uint32_t _version;
		        int _datalensize() const {
		        	if (this->_version == 1) {return 2;}
		        	else if (this->_version == 2) {return 4;}
		        	return 0;
		        }
		        bool _end_key() {
		        	if (!this->_writing || !this->_inkey){return false;}
		        	auto endpos = this->ostr.tellp();
		        	auto kdsz = size_t(endpos - this->_ddf_kdstartpos);
		        	this->ostr.seekp(this->_ddf_kdstartpos);
		        	this->ostr.seekp(-(this->_datalensize()), std::ios::cur);
		        	switch (this->_version) {
		        		case 1: this->pack<size_t, uint16_t>(&kdsz, 1, true); break;
		        		case 2: this->pack<size_t, uint32_t>(&kdsz, 1, true); break;
		        	}
		        	this->ostr.seekp(endpos);
		        	this->_inkey = false;
		        	return true;
		        }
		    public:
		        template <class VT, class WT=VT>
		        void pack(const VT* values, const size_t n=1, const bool _le=_io::hostLE) {
		        	binio::pack<VT,WT>(this->ostr, values, n, _le);
		        }
		        _ddfwriter_t(std::ostream& _ostr, const uint32_t version=2, const ddfkey_t _verkey=_DMD_DDF_VERKEY, const ddfkey_t _termkey=_DMD_DDF_TERMKEY) :
		            ostr(_ostr),
		            _version(version),
		            verkey(_verkey),
		            termkey(_termkey)
		        {
		        	this->_ddf_startpos = this->ostr.tellp();
		        	this->pack<char>(this->verkey+0, 8);
		        	this->pack(&(this->_version), 1, true);
		        }
		        bool newkey(const ddfkey_t key) {
		        	if (!this->_writing){return false;}
		        	this->_end_key();

		        	this->_curkey = key;
		        	this->pack<char>(this->_curkey+0, 8);
		        	this->ostr.write("\0\0\0\0", this->_datalensize());
		        	this->_ddf_kdstartpos = this->ostr.tellp();

		        	this->_inkey = true;

		        	return true;
		        }
		        size_t terminate() {
		        	if (!this->_writing){return 0;}
		        	this->_end_key();

                    this->pack(this->termkey+0, 8);

		        	this->_writing = false;
		        	return size_t(this->ostr.tellp() - this->_ddf_startpos);
		        }
		        ~_ddfwriter_t() {this->terminate();}
		};

		struct _ddfwalker_t {
		    private:
		        std::istream& istr;
		        const ddfkey_t verkey, termkey;
		        ddfkey_t _key;
		        bool _has_set_version = false;
		        uint32_t _version = 0, _datalen = 0;
		        std::streampos datastart, dataend;
		    public:
		        const uint32_t& version = _version;
		        const uint32_t& datalen = _datalen;
		        const ddfkey_t& key = _key;

		        template <class VT, class RT=VT>
		        VT* unpack(VT* dst, const size_t n, const bool le=_io::hostLE)
		        {
		        	return _io::unpack<VT,RT>(dst,this->istr,n,le);
		        }

		        bool adv()
		        {
		            std::stringstream errss;
		            this->datastart = this->istr.tellg();
		        	this->unpack(this->_key+0, 8);
		        	if (this->_key == this->termkey){return false;}
		        	else if (this->_key == this->verkey && !this->_has_set_version)
		        	{
		        		this->_has_set_version = true;
		        		this->unpack(&(this->_version), 1, true);
		        		if (this->_version == 0 || this->_version >= 3)
		        		{
		        			errss << "Unknown DDF version number: " << this->_version;
		        			throw std::runtime_error(errss.str());
		        		}
		        		return this->adv();
		        	}
		        	else if (!this->_has_set_version)
		        	{
		        	    errss << "Reached nonspecial DDF key without a known DDF version number!";
		        		throw std::runtime_error(errss.str());
		        	}
		        	else if (this->_version == 1)
		        	{
		        		this->unpack<uint32_t, uint16_t>(&(this->_datalen), 1, true);
		        	}
		        	else if (this->_version == 2)
		        	{
		        		this->unpack(&(this->_datalen), 1, true);
		        	}
		        	this->dataend = size_t(this->istr.tellg()) + this->_datalen;
		        	return true;
		        }

		        void skip2next()
		        {
		        	this->istr.seekg(this->dataend, std::ios::beg);
		        }
		        _ddfwalker_t(std::istream& _istr, const ddfkey_t _verkey=_DMD_DDF_VERKEY, const ddfkey_t _termkey=_DMD_DDF_TERMKEY) : istr(_istr), verkey(_verkey), termkey(_termkey) {}

		};


		using dmdnodetype = _array1dTN<char, 4>;

		const dmdnodetype _NODETYPE_EMPTY = "EMPT";
		const dmdnodetype _NODETYPE_MESH = "MESH";

		template <class T, size_t N>
		struct _vectorTN {
		    T _elements[N];
		    size_t size() const {return N;}
		    T* data() {return this->_elements + 0;}
		    const T* data() const {return this->_elements + 0;}
		    T& operator[](const int i) {
		    	return this->_elements[i];
		    }
		    const T& operator[](const int i) const {
	   	    	return this->_elements[i];
	   	    }
		    _vectorTN() {}
		    _vectorTN(const _vectorTN<T,N>& o) {std::copy_n(o.data(), N, this->data());}
		    _vectorTN(const size_t num, const T value=T())
		    {
		    	std::fill_n(this->data(), num, value);
		    }
		    _vectorTN(const std::initializer_list<T>& il)
		    {
		    	std::copy_n(il.begin(), std::min(il.size(), N), this->_elements);
		    }
		    ~_vectorTN() {}
		};
		template <class T, size_t M, size_t N> using _matrixTMN = _vectorTN<_vectorTN<T, N>, M>;

		using dvector2_t = _vectorTN<double, 2>;
		using dvector3_t = _vectorTN<double, 3>;
		using dvector4_t = _vectorTN<double, 4>;
		using drgb_t = _vectorTN<double, 3>;
		using dmatrix33_t = _matrixTMN<double, 3, 3>;
		using dataid_t = size_t;

		struct DMDFile;
		struct _DMDFileDataBase;

		enum class dmd_datatype : int {
		    unknown = 0,
		    node = 1,
		    teximg = 2,
		    texdata = 3,
		    material = 4,
		    mesh = 5,
		    submdl = 6
		};

        enum class dmd_physics_shape : uint16_t {
            none = 0,
            trimesh = 1
        };

        typedef uint32_t dmd_physics_collmask;

		enum class texblend : uint8_t {
		    mix = 0,
		    add = 1,
		    mul = 2
		};
		enum class matblend : uint8_t {
			none = 0,
			alpha_blend = 1,
			add = 2
		};
		enum class texmap : uint16_t {
		    uvmap = 0,
		    mirror_ball_reflection = 1,
            view = 2
		};
		enum class facecullmode : uint8_t {
            none = 0,
            back = 1,
            front = 2
        };
        enum class texsrc : uint32_t {
        	image = 0,
        	noise = 1,
        	equirect = 2
        };
        enum class bumpmapspace : uint8_t {
            object = 0,
            tangent = 1
        };
		using datatbl_t = std::unordered_map<dataid_t, _DMDFileDataBase*>;

		struct _DMDFileDataBase {
			DMDFile* dmdfile = nullptr;
			dataid_t uid = 0;

			uint32_t version = 0;

			virtual operator dmd_datatype() const {return dmd_datatype::unknown;}
			virtual ~_DMDFileDataBase() = default;
		};
        class DMD_FXPointer {
            public:
                //_DMDFileDataBase* basedata = nullptr;
                bool has_fx = false;
                enum class fxdatatype : uint32_t {
                    i32 = 0,
                    u32 = 1,
                    f32 = 2,
                    i64 = 3,
                    u64 = 4,
                    f64 = 5
                };

                union fxdataelem {
                    int32_t i32;
                    uint32_t u32;
                    float f32;
                    int64_t i64;
                    uint64_t u64;
                    double f64;
                    char _raw[8];
                };

                struct fxargvalue {
                    fxdatatype type = fxdatatype::i32;
                    std::vector<fxdataelem> elements = {};
                };

                std::unordered_map<std::string, fxargvalue> fxargs = {};
                uint32_t fxcat = 0, fxnum = 0, fxdatsz_infile = 0;

                template <typename T, typename DST=T*>
                bool getargval(DST& dst, const std::string argname, const size_t idx=0, const size_t num=1, const size_t dststart=0) const {
                    if (this->fxargs.count(argname) == 0){return false;}
                    const auto& fxarg = this->fxargs.at(argname);
                    size_t i = 0;
                    for (size_t cidx = idx, didx = dststart; i < num; i++, cidx++, didx++){
                        const auto& e = fxarg.elements.at(cidx);
                        T& cdst = dst[didx];
                        switch (fxarg.type) {
                            case fxdatatype::i32: cdst = T(e.i32); break;
                            case fxdatatype::u32: cdst = T(e.u32); break;
                            case fxdatatype::f32: cdst = T(e.f32); break;
                            case fxdatatype::i64: cdst = T(e.i64); break;
                            case fxdatatype::u64: cdst = T(e.u64); break;
                            case fxdatatype::f64: cdst = T(e.f64); break;
                            default: ;
                        }
                    }
                    return true;
                }
                void init() {
                    //this->basedata = _basedata;
                    this->has_fx = false;
                    this->fxargs.clear();
                    this->fxcat = this->fxnum = this->fxdatsz_infile = 0;
                }

                void read(std::istream& f, const std::vector<std::string>& strlist) {
                    this->init();
                    binio::unpack(&(this->fxcat), f, 3, true);
                    for (_ddfwalker_t itr(f); itr.adv();) {
                        if (itr.key == ddfkey_t("FXARG")){
                            uint32_t version = 0, nameindex = 0, num_elems = 1;
                            fxargvalue av = {};
                            itr.unpack(&version, 1, true);
                            itr.unpack(&nameindex, 1, true);
                            itr.unpack((uint32_t*)&(av.type), 1, true);
                            itr.unpack(&num_elems, 1, true);
                            const std::string argname = strlist[nameindex];
                            std::cout << "new fxarg: name=" << std::quoted(argname) << ", type=" << uint32_t(av.type) << ", elems={ ";
                            for (uint32_t i = 0; i < num_elems; i++) {
                                fxdataelem ne = {};
                                switch (av.type) {
                                    case fxdatatype::i32: itr.unpack(&ne.i32, 1, true); std::cout << ne.i32 << " "; break;
                                    case fxdatatype::u32: itr.unpack(&ne.u32, 1, true); std::cout << ne.u32 << " "; break;
                                    case fxdatatype::f32: itr.unpack(&ne.f32, 1, true); std::cout << ne.f32 << " "; break;
                                    case fxdatatype::i64: itr.unpack(&ne.i64, 1, true); std::cout << ne.i64 << " "; break;
                                    case fxdatatype::u64: itr.unpack(&ne.u64, 1, true); std::cout << ne.u64 << " "; break;
                                    case fxdatatype::f64: itr.unpack(&ne.f64, 1, true); std::cout << ne.f64 << " "; break;
                                    default: ;
                                }
                                av.elements.push_back(ne);
                            }
                            std::cout << "}\n";
                            this->fxargs[argname] = av;
                        }
                        else {
                            itr.skip2next();
                        }
                    }
                    this->has_fx = true;
                }

        };
		struct DMD_SubModel;
		struct DMD_Node : public _DMDFileDataBase {
		    DMD_Node* parent = nullptr;
		    bool visible = true;
		    std::vector<DMD_Node*> children;
		    std::string name;
		    dmdnodetype nodetype;
		    DMD_SubModel* submdl = nullptr;
		    dvector3_t local_position = {0,0,0}, local_scale = {1,1,1};
		    dmatrix33_t local_orientation = {
		        {1, 0, 0},
		        {0, 1, 0},
		        {0, 0, 1}
		    };
            DMD_FXPointer nodefx = {};
            dmd_physics_shape physics_shape = dmd_physics_shape::none;
            dmd_physics_collmask collision_mask = 0U;
		    operator dmd_datatype() const {return dmd_datatype::node;}

		};

		enum class texorigin : uint8_t {
			tl = 0,
			tr = 1,
			bl = 2,
			br = 3
		};

		struct teximglvl_t {
		    uint32_t width = 0, height = 0, cpp = 0;
		    texorigin origin = texorigin::bl;
		    uint8_t* pixels = nullptr;

		    size_t size() const {return this->width * this->height * this->cpp;}
		    size_t npx() const {return this->width * this->height;}

		    size_t xy2offs(const int _x, const int _y) const
		    {
		        int x = std::min(std::max(_x, 0), int(this->width) - 1);
		        int y = std::min(std::max(_y, 0), int(this->height) - 1);
		        size_t idx = 0;
		    	switch (this->origin)
		    	{
		    		case texorigin::tr:
		    		{
		    			idx = (y * this->width) + (this->width - 1 - x);
		    			break;
		    		}
		    		case texorigin::bl:
		    		{
		    			idx = ((this->height - 1 - y) * this->width) + (x);
		    			break;
		    		}
		    		case texorigin::br:
		    		{
		    			idx = ((this->height - 1 - y) * this->width) + (this->width - 1 - x);
		    			break;
		    		}
		    		default:
		    		{
		    			idx = (y * this->width) + x;
		    			break;
		    		}
		    	}
		    	return idx * this->cpp;
		    }
		    teximglvl_t& close()
		    {
		        if (this->pixels != nullptr){delete[] this->pixels;}
		        this->pixels = nullptr;
		    	this->width = this->height = this->cpp = 0;
		    	return *this;
		    }
		    teximglvl_t& init(const uint8_t* px, const uint32_t nw, const uint32_t nh, const uint32_t ncpp)
		    {
		        this->close();
		    	if ((nw*nh*ncpp) == 0){throw std::runtime_error("total size MUST be > 0!");}
		    	this->width = nw; this->height = nh; this->cpp = ncpp;
		    	this->pixels = new uint8_t[this->size()];
		    	if (px != nullptr)
		    	    memcpy(this->pixels, px, this->size());
		    	else
		    	    memset(this->pixels, 0, this->size());
		    	return *this;
		    }

		    teximglvl_t& read_rim(std::istream& istr)
		    {
		        char ident_rd[7];
		        bool indexed;
		        _io::unpack<char>(ident_rd, istr, 7);
		        if (!memcmp(ident_rd, "RAWIMG\0", 7)){indexed = false;}
		        else if (!memcmp(ident_rd, "RAWIMGP", 7)){indexed = true;}
		        else {throw std::runtime_error("Not a RIM!");}

		        uint32_t dims[3];
		        _io::unpack<uint32_t>(dims, istr, 3, true);
		        uint8_t pal[1024];
		        if (indexed){_io::unpack<uint8_t>(pal, istr, 256*dims[2]);}
		        this->init(nullptr, dims[0], dims[1], dims[2]);
		        const size_t sz = this->size();
		        const size_t npx = this->npx();

		        if (indexed)
		        {
		        	for (size_t i = 0; i < npx; i++)
		        	{
		        	    size_t palind;
		        	    _io::unpack<size_t, uint8_t>(&palind, istr, 1);
		        	    memcpy(this->pixels + (i * dims[2]), pal + (palind * dims[2]), dims[2]);

		        	}
		        }
		        else
		        {
		        	istr.read((char*)(this->pixels), sz);
		        }
		    	return *this;
		    }
		    size_t write_rim(std::ostream& ostr) const {
				auto startpos = ostr.tellp();
				ostr.write("RAWIMG\0", 7);
				uint32_t dims[3] = {this->width, this->height, this->cpp};
				binio::pack(ostr, dims, 3, true);
				ostr.write((char*)this->pixels, this->width * this->height * this->cpp);
				auto endpos = ostr.tellp();
				return size_t(endpos - startpos);
			}
		    teximglvl_t& vflip()
		    {
		    	teximglvl_t cp(*this);
		    	for (uint32_t y = 0; y < this->height; y++)
		    	{
		    		uint32_t iy = this->height - 1 - y;
		    		memcpy(this->pixels + (this->cpp * this->width * y), cp.pixels + (cp.cpp * cp.width * iy), this->cpp * this->width);
		    	}
		    	return *this;
		    }
		    teximglvl_t() {}
		    teximglvl_t(const teximglvl_t& other)
		    {
		        this->origin = other.origin;
		        if (other.pixels != nullptr){this->init(other.pixels, other.width, other.height, other.cpp);}
		    }
		    teximglvl_t(const teximglvl_t& other, const texorigin _origin)
		    {
		    	this->origin = _origin;
		    	this->init(nullptr, other.width, other.height, other.cpp);
		    	const int w = other.width, h = other.height;
		    	for (int y = 0; y < h; y++)
		    	{
		    		for (int x = 0; x < w; x++)
		    		{
		    			const uint8_t* otherpx = other.pixels + other.xy2offs(x, y);
		    			uint8_t* mypx = this->pixels + this->xy2offs(x, y);
		    			memcpy(mypx, otherpx, this->cpp);
		    		}
		    	}
		    }
		    ~teximglvl_t(){
		        std::cout << "djutil::dmd::teximglvl_t deleted!\n";
		        this->close();
            }
		};
		struct DMD_TexImage : public _DMDFileDataBase {
		    std::vector<teximglvl_t> images;
		    operator dmd_datatype() const {return dmd_datatype::teximg;}
		};

		struct texaffect_t {
		    bool use = false;
		    double value = 0.0;

		    void readf(std::istream& istr)
		    {
		    	_io::unpack<bool, uint8_t>(&this->use, istr, 1, true);
		    	_io::unpack(&this->value, istr, 1, true);
		    }
		    void writef(std::ostream& ostr) const {
				uint8_t _use(this->use);
				binio::pack(ostr, &_use, 1, true);
				binio::pack(ostr, &this->value, 1, true);
			}
		};

		struct DMD_TexData : public _DMDFileDataBase {
		    DMD_TexImage* teximg = nullptr;
		    texaffect_t
		        affects_diffuse_color,
		        affects_diffuse_alpha,
		        affects_diffuse_intensity,
		        affects_specular_color,
		        affects_specular_intensity,
		        affects_specular_hardness,
		        affects_specular_alpha,
                affects_texcoord_offset,
                affects_normal_vector,
                affects_emit_amount,
                affects_ambient_color
		    ;
		    texblend blendtype = texblend::mix;
		    uint32_t uvmap_index = 0;
		    texmap maptype = texmap::uvmap;
            texsrc texsource = texsrc::image;
            DMD_FXPointer texfx = {};
            bumpmapspace bumpspace = bumpmapspace::object;

            bool use_rgb2intensity = false;
            dvector3_t rgb2intensity_color = {1,1,1};

		    operator dmd_datatype() const {return dmd_datatype::texdata;}
		};

		struct dmd_texslot_t {
		    bool use = false;
		    DMD_TexData* texdata = nullptr;
		};

		struct DMD_Material : public _DMDFileDataBase {
			dmd_texslot_t texture_slots[8];
			dvector3_t
			    base_diffuse_color,
			    base_specular_color,
			    base_ambient_color
			;
			double
			    base_diffuse_intensity,
	            base_specular_intensity,
		        base_specular_hardness,
		        base_diffuse_alpha,
		        base_specular_alpha,
		        base_emit_amount
			;
			bool shadeless;
            facecullmode facecull = facecullmode::back;
			matblend blendtype;
            DMD_FXPointer matfx = {};
			operator dmd_datatype() const {return dmd_datatype::material;}

		};

		struct DMD_Mesh : public _DMDFileDataBase {
	        std::vector<dvector3_t> vertices;
	        std::vector<dvector3_t> normals;
	        std::vector<std::vector<dvector2_t>> uvmaps;

	        std::vector<dvector3_t> vertex_colors;

            std::vector<dvector3_t> tangents;
            std::vector<dvector3_t> binormals;

	        std::vector<uint32_t> indices;

	        DMD_Material* material = nullptr;

	        operator dmd_datatype() const {return dmd_datatype::mesh;}

	        DMD_Mesh* blank_mesh(const size_t num_faces, const size_t num_vertices, const size_t num_uvmaps) {
				this->vertices.clear(); this->vertices.resize(num_vertices);
				this->vertex_colors.clear(); this->vertex_colors.resize(num_vertices);
				this->normals.clear(); this->normals.resize(num_vertices);
				this->uvmaps.clear(); this->uvmaps.resize(num_uvmaps);
				this->tangents.clear(); this->tangents.resize(num_vertices);
				this->binormals.clear(); this->binormals.resize(num_vertices);
				for (auto& uvmap : this->uvmaps) {uvmap.resize(num_vertices);}
				this->indices.clear(); this->indices.resize(num_faces);
				return this;
			}
		};

	    struct DMD_SubModel : public _DMDFileDataBase {
		    std::vector<DMD_Mesh*> meshes;
		    operator dmd_datatype() const {return dmd_datatype::submdl;}
		};

		template <typename T>
		size_t _findfirstidx(const std::vector<T>& v, const T& e) {
			return (std::find(v.cbegin(), v.cend(), e) - v.cbegin());
		}
		struct DMDFile {
		    _array1dTN<char, 8> dmdident;
		    uint32_t dmdversion = 0;
		    dataid_t cur_uid = 1;
		    std::unordered_map<dataid_t, _DMDFileDataBase*> datamap;

		    std::vector<DMD_Node*> dmdnodes;
		    std::vector<DMD_Mesh*> dmdmeshes;
		    std::vector<DMD_SubModel*> dmdsubmodels;
		    std::vector<DMD_TexImage*> dmdteximages;
		    std::vector<DMD_TexData*> dmdtextures;
		    std::vector<DMD_Material*> dmdmaterials;

		    bool take_ownership(_DMDFileDataBase* data)
		    {
		    	if (data->dmdfile == this){return false;}
		    	else
		    	{
		    	    if (data->dmdfile != nullptr){throw std::runtime_error("Only orphan dataobjects can be taken ownership of!");}
		    	    data->uid = this->cur_uid; this->cur_uid++;
		    	    data->dmdfile = this;
		    	    this->datamap[data->uid] = data;
		    		return true;
		    	}
		    }

		    template <class T>
		    T* new_dataobj()
		    {
		    	T* obj = new T();
		    	this->take_ownership(obj);
		    	return obj;
		    }
		    template <class T>
		    void _fill_list(std::vector<T*>& lst, const size_t num)
		    {
		        if (num == 0){return;}
		    	lst.resize(num);
		    	for (T*& elem : lst){elem = this->new_dataobj<T>();}
		    }
		    template <class T>
		    void _ensure_in_list(std::vector<T*>& lst, T* data) {
				if (std::find(lst.begin(), lst.end(), data) == lst.end()){
					lst.push_back(data);
				}
			}
			DMD_Node* init_rootnode() {
				if (this->dmdnodes.size() == 0){this->dmdnodes.push_back(nullptr);}
				DMD_Node*& root = this->dmdnodes.front();
				if (root == nullptr){root = this->new_dataobj<DMD_Node>();}
				root->version = 0;
				root->nodetype = _NODETYPE_EMPTY;
				root->name = "";
				root->parent = nullptr;
		    	return root;
			}
		    void ensuresAllDataInLists() {
				for (auto& kv : this->datamap) {
					auto& uid = kv.first;
					_DMDFileDataBase*& data = kv.second;
					switch (dmd_datatype(*data)) {
						case dmd_datatype::node: this->_ensure_in_list<DMD_Node>(this->dmdnodes, (DMD_Node*)data); break;
						case dmd_datatype::mesh: this->_ensure_in_list<DMD_Mesh>(this->dmdmeshes, (DMD_Mesh*)data); break;
						case dmd_datatype::submdl: this->_ensure_in_list<DMD_SubModel>(this->dmdsubmodels, (DMD_SubModel*)data); break;
						case dmd_datatype::material: this->_ensure_in_list<DMD_Material>(this->dmdmaterials, (DMD_Material*)data); break;
						case dmd_datatype::texdata: this->_ensure_in_list<DMD_TexData>(this->dmdtextures, (DMD_TexData*)data); break;
						case dmd_datatype::teximg: this->_ensure_in_list<DMD_TexImage>(this->dmdteximages, (DMD_TexImage*)data); break;
						default: ;
					}
				}
			}
		    void close()
		    {
		    	for (auto kvp = this->datamap.begin(); kvp != this->datamap.end(); kvp++)
		    	{
		    		//dataid_t uid = kvp->first;
		    		//_DMDFileDataBase* obj = kvp->second;
                    delete kvp->second;
                    kvp->second = nullptr;
		    	}
		    	this->datamap.clear();
		    	this->dmdversion = 0;

		    	this->dmdnodes.clear();
		    	this->dmdmeshes.clear();
		    	this->dmdsubmodels.clear();
		    	this->dmdteximages.clear();
		    	this->dmdtextures.clear();
		    	this->dmdmaterials.clear();
		    }

		    void load(std::istream& istr)
		    {
		        std::stringstream errss;
		    	this->close();
		    	_io::unpack(this->dmdident+0, istr, 8);
		    	if (this->dmdident != _DMD_FILE_MAGIC)
		    	{
		    	    errss << "Bad DMD identifier string: " << '"' << this->dmdident << '"' << '\n';
		    		throw std::runtime_error(errss.str());
		    	}
		    	//std::cout << "dmd identifier string = " << this->dmdident << '\n';
		    	_io::unpack(&(this->dmdversion), istr, 1, true);
		    	//std::cout << "dmd fileversion = " << this->dmdversion << '\n';


		    	struct _hdrkeydata {
		    	    ddfkey_t key;
		    	    int subsect;
		    	    uint32_t num = 0;
		    	};

		    	_hdrkeydata hdrkeys[] = {
		    	    {"NSTRS", 1},
	    	        {"NNODES", 2},
	    	        {"NMESHES", 3},
	    	        {"NSMDLS", 4},
	    	        {"NMATS", 5},
	    	        {"NTEXDATS", 6},
	    	        {"NTEXIMGS", 7},
	    	        {"XDATSIZE", 8}
		    	};

		    	size_t numhdrkeys = sizeof(hdrkeys)/sizeof(_hdrkeydata);
		    	if (this->dmdversion == 0){numhdrkeys--;}
		    	bool hdrgotkey;
		    	for (_ddfwalker_t ddf1(istr); ddf1.adv();)
		    	{
		    	    //std::cout << std::quoted(std::string(ddf1.key, ddf1.key.size())) << '\n';
		    	    hdrgotkey = false;
		    	    for (size_t i = 0; i < numhdrkeys; i++)
		    	    {
		    	    	if (ddf1.key == hdrkeys[i].key)
		    	    	{
		    	    		ddf1.unpack(&(hdrkeys[i].num), 1, true);
		    	    		//std::cout << "hdrkey: <" << hdrkeys[i].key << "> = " << hdrkeys[i].num << '\n';
		    	    		hdrgotkey = true;
		    	    		break;
		    	    	}
		    	    }
		    	    if (!hdrgotkey){ddf1.skip2next();}
		    	}

		    	std::vector<std::string> strlist;
	            strlist.resize(hdrkeys[0].num);
	            size_t strindex = 0;
		    	for (std::string& str : strlist)
		    	{
		    		size_t cslen; _io::unpack<size_t, uint16_t>(&cslen, istr, 1, true);
		    		str.resize(cslen); _io::unpack(&(str[0]), istr, cslen);
		    		//std::cout << "stringlist[" << strindex << "] = " << std::quoted(str) << '\n';
		    		strindex++;
		    	}
		    	this->_fill_list(this->dmdnodes, hdrkeys[1].num + 1);
		    	this->_fill_list(this->dmdmeshes, hdrkeys[2].num);
		    	this->_fill_list(this->dmdsubmodels, hdrkeys[3].num);
		    	this->_fill_list(this->dmdmaterials, hdrkeys[4].num);
		    	this->_fill_list(this->dmdtextures, hdrkeys[5].num);
		    	this->_fill_list(this->dmdteximages, hdrkeys[6].num);

		    	DMD_Node* root = this->init_rootnode();

		    	size_t nodenum = 1;

				for (auto it = this->dmdnodes.begin()+1; it != this->dmdnodes.end(); it++)
			    {
			        DMD_Node*& node = *it;
			        _io::unpack(&(node->version), istr, 1, true);
			        //std::cout << "dmdnodes[" << nodenum << "]:\n ->version = " << node->version << "\n";
			        for (_ddfwalker_t ddf2(istr); ddf2.adv();)
			        {
			            if (ddf2.key == ddfkey_t("PNINDEX"))
			            {
			                uint32_t pni; ddf2.unpack(&pni, 1, true);
			            	node->parent = this->dmdnodes[pni];
			            	//std::cout << " ->parent = dmdnodes[" << pni << "];\n";
			            	node->parent->children.push_back(node);
			            }
			            else if (ddf2.key == ddfkey_t("NINDEX"))
			            {
			            	uint32_t ni; ddf2.unpack(&ni, 1, true);
			            	node->name = strlist[ni];
			            	//std::cout << " ->name = " << std::quoted(node->name) << "; (stringlist[" << ni << "])\n";
			            }
			            else if (ddf2.key == ddfkey_t("NTYPE"))
			            {
			            	ddf2.unpack<char>(node->nodetype, 4);
			            }
			            else if (ddf2.key == ddfkey_t("LOCPOS"))
			            {
			            	ddf2.unpack(node->local_position.data(), 3, true);
			            }
			            else if (ddf2.key == ddfkey_t("LOCSCL"))
	   		            {
	   		            	ddf2.unpack(node->local_scale.data(), 3, true);
	   		            }
			            else if (ddf2.key == ddfkey_t("LOCORN"))
			            {
			            	ddf2.unpack(node->local_orientation[0].data(), 3, true);
			            	ddf2.unpack(node->local_orientation[1].data(), 3, true);
			            	ddf2.unpack(node->local_orientation[2].data(), 3, true);
			            }
			            else if (ddf2.key == ddfkey_t("SUBMDL"))
			            {
			            	uint32_t smi; ddf2.unpack(&smi, 1, true);
			            	node->submdl = this->dmdsubmodels[smi];
			            }
			            else if (ddf2.key == ddfkey_t("VISIBLE"))
			            {
							node->visible = istr.get() > 0;
						}
                        else if (ddf2.key == ddfkey_t("NODEFX")) {
                            node->nodefx.read(istr, strlist);
                        }
                        else if (ddf2.key == ddfkey_t("PSHAPE")) {
                            ddf2.unpack((uint16_t*)&(node->physics_shape), 1, true);
                        }
                        else if (ddf2.key == ddfkey_t("COLLMASK")) {
                            ddf2.unpack(&(node->collision_mask), 1, true);
                        }
			            else
			            {
			                std::cout << "dmdnodes[" << nodenum << "]: skip key <" << ddf2.key << "> with datalen=" << ddf2.datalen << '\n';
			            	ddf2.skip2next();
			            }
			        }
			        nodenum++;
			    }

			    size_t meshnum = 0;
			    for (DMD_Mesh*& mesh : this->dmdmeshes)
			    {
			        //std::cout << "dmdmeshes[" << meshnum << "]:\n";
			    	_io::unpack(&(mesh->version), istr, 1, true);
			    	//std::cout << " ->version = " << mesh->version << ";\n";
			    	uint32_t nverts_and_nuvmaps[2], nfaces;
			    	uint8_t uvmode;
			    	_io::unpack<uint32_t>(nverts_and_nuvmaps, istr, 2, true);
			    	//std::cout << " ->vertices.size() = " << nverts_and_nuvmaps[0] << ";\n";
			    	//std::cout << " ->uvmaps.size() = " << nverts_and_nuvmaps[1] << ";\n";
			    	_io::unpack(&uvmode, istr, 1, true);
			    	//std::cout << " uvmode = " << int(uvmode) << ";\n";
			    	_io::unpack(&nfaces, istr, 1, true);
			    	//std::cout << " ->indices.size() = " << nfaces*3 << ";\n";

			    	mesh->vertices.resize(nverts_and_nuvmaps[0]);
			    	mesh->normals.resize(nverts_and_nuvmaps[0]);
			    	mesh->vertex_colors.resize(nverts_and_nuvmaps[0]);
                    mesh->tangents.resize(nverts_and_nuvmaps[0]);
                    mesh->binormals.resize(nverts_and_nuvmaps[0]);
			    	mesh->uvmaps.resize(nverts_and_nuvmaps[1]);
			    	for (auto& uvmap : mesh->uvmaps)
			    	{
			    		uvmap.resize(nverts_and_nuvmaps[0]);
			    	}
			    	mesh->indices.resize(nfaces*3);
			    	for (_ddfwalker_t ddf2(istr); ddf2.adv();)
			    	{
			    		if (ddf2.key == ddfkey_t("VERTS"))
			    		{
			    			ddf2.unpack((double*)(mesh->vertices.data()), mesh->vertices.size() * 3, true);
			    		}
			    		else if (ddf2.key == ddfkey_t("COLORS"))
			    		{
			    			ddf2.unpack((double*)(mesh->vertex_colors.data()), mesh->vertex_colors.size() * 3, true);
			    		}
			    	    else if (ddf2.key == ddfkey_t("NORMALS"))
			    		{
			    			ddf2.unpack((double*)(mesh->normals.data()), mesh->normals.size() * 3, true);
			    		}
			    		else if (ddf2.key == ddfkey_t("TANGENTS"))
			    		{
			    			ddf2.unpack((double*)(mesh->tangents.data()), mesh->tangents.size() * 3, true);
			    		}
			    		else if (ddf2.key == ddfkey_t("BINORMS"))
			    		{
			    			ddf2.unpack((double*)(mesh->binormals.data()), mesh->binormals.size() * 3, true);
			    		}
			    		else if (ddf2.key == ddfkey_t("UVMAPS"))
			    		{
			    		    for (auto& uvmap : mesh->uvmaps)
			    		    {
			    		    	ddf2.unpack((double*)(uvmap.data()), uvmap.size() * 2, true);
			    		    }
			    		}
			    		else if (ddf2.key == ddfkey_t("FACES"))
			    		{
			    			ddf2.unpack(mesh->indices.data(), mesh->indices.size(), true);
			    		}
			    		else if (ddf2.key == ddfkey_t("MATINDEX"))
			    		{
			    			uint32_t mmi; ddf2.unpack(&mmi, 1, true);
			    			//std::cout << " ->material = dmdmaterials[" << mmi << "];\n";
			    			mesh->material = this->dmdmaterials[mmi];
			    		}
			    		else
			    		{
			    			std::cout << "dmdmeshes[" << meshnum << "]: skip key <" << ddf2.key << "> with datalen=" << ddf2.datalen << "\n";
			    			ddf2.skip2next();
			    		}
			    	}
			    	meshnum++;
			    }

		    	size_t submdlnum = 0;
		    	for (DMD_SubModel*& submdl : this->dmdsubmodels)
		    	{
		    	    //std::cout << "dmdsubmodels[" << submdlnum << "];\n";
		    	    _io::unpack(&(submdl->version), istr, 1, true);
		    	    //std::cout << " ->version = " << submdl->version << ";\n";

		    	    for (_ddfwalker_t ddf2(istr); ddf2.adv();)
		    	    {
		    	    	if (ddf2.key == ddfkey_t("MESHES"))
		    	    	{
		    	    		uint32_t nmeshes, csmi, smmi = 0; ddf2.unpack(&nmeshes, 1, true);
		    	    		//std::cout << " ->meshes.size() = " << nmeshes << ";\n";
		    	    		submdl->meshes.resize(nmeshes);
		    	    		for (DMD_Mesh*& submesh : submdl->meshes)
		    	    		{
		    	    			ddf2.unpack(&csmi, 1, true);
		    	    			//std::cout << " ->meshes[" << smmi << "] = dmdmeshes[" << csmi << "];\n";
		    	    			submesh = this->dmdmeshes[csmi];
		    	    			smmi++;
		    	    		}
		    	    	}
		    	    	else
		    	    	{
		    	    		std::cout << "dmdsubmodels[" << submdlnum << "]: skip key <" << ddf2.key << "> with datalen=" << ddf2.datalen << "\n";
		    	    	    ddf2.skip2next();
		    	    	}
		    	    }
		    		submdlnum++;
		    	}

		    	size_t matnum = 0;
		    	for (DMD_Material*& material : this->dmdmaterials)
		    	{
		    	    //std::cout << "dmdmaterials[" << matnum << "];\n";
		    	    _io::unpack(&(material->version), istr, 1, true);
		    	    //std::cout << " ->version = " << material->version << ";\n";
		    	    for (_ddfwalker_t ddf2(istr); ddf2.adv();)
		    	    {
		    	    	if (ddf2.key == ddfkey_t("TEXSLOT"))
		    	    	{
		    	    		uint32_t tsi, tdi;
		    	    		uint8_t in_use;
		    	    		ddf2.unpack(&tsi, 1, true);
		    	    		ddf2.unpack(&in_use, 1, true);
		    	    		ddf2.unpack(&tdi, 1, true);

		    	    		//std::cout << " ->texture_slots[" << tsi << "] = {\n";
		    	    		//std::cout << "        .use=" << (in_use ? "true" : "false") << ",\n";
		    	    		//if (in_use) {std::cout << "        .texdata=dmdtextures[" << tdi << "]" << "\n";}
		    	    		//else        {std::cout << "        .texdata=nullptr\n";}
		    	    		//std::cout << "  };\n";

		    	    		material->texture_slots[tsi].use = bool(in_use);
		    	    	    material->texture_slots[tsi].texdata = (in_use ? this->dmdtextures[tdi] : nullptr);
		    	    	}
		    	    	else if (ddf2.key == ddfkey_t("SHDLESS"))
		    	    	{
		    	    		ddf2.unpack<bool, uint8_t>(&(material->shadeless), 1, true);
		    	    	}
		    	    	else if (ddf2.key == ddfkey_t("BDC")){ddf2.unpack(material->base_diffuse_color.data(), 3, true);}
		    	    	else if (ddf2.key == ddfkey_t("BSC")){ddf2.unpack(material->base_specular_color.data(), 3, true);}
		    	    	else if (ddf2.key == ddfkey_t("BAC")){ddf2.unpack(material->base_ambient_color.data(), 3, true);}
		    	    	else if (ddf2.key == ddfkey_t("BDA")){ddf2.unpack(&material->base_diffuse_alpha, 1, true);}
		    	    	else if (ddf2.key == ddfkey_t("BSA")){ddf2.unpack(&material->base_specular_alpha, 1, true);}
		    	    	else if (ddf2.key == ddfkey_t("BDI")){ddf2.unpack(&material->base_diffuse_intensity, 1, true);}
		    	    	else if (ddf2.key == ddfkey_t("BSI")){ddf2.unpack(&material->base_specular_intensity, 1, true);}
		    	    	else if (ddf2.key == ddfkey_t("BSH")){ddf2.unpack(&material->base_specular_hardness, 1, true);}
		    	    	else if (ddf2.key == ddfkey_t("BEA")){ddf2.unpack(&material->base_emit_amount, 1, true);}
		    	    	else if (ddf2.key == ddfkey_t("BLENDT")){ddf2.unpack<matblend, uint8_t>(&material->blendtype, 1, true);}
		    	    	else if (ddf2.key == ddfkey_t("FACECULL")){ddf2.unpack<facecullmode, uint8_t>(&material->facecull, 1, true);}
                        else if (ddf2.key == ddfkey_t("MATFX")) {
                            material->matfx.read(istr, strlist);
                        }
		    	    	else
		    	    	{
		    	    		std::cout << "dmdmaterials[" << matnum << "]: skip key <" << ddf2.key << "> with datalen=" << ddf2.datalen << "\n";
		    	    	    ddf2.skip2next();
		    	    	}
		    	    }
		    		matnum++;
		    	}

		    	size_t texdatnum = 0;

		    	for (DMD_TexData*& texdat : this->dmdtextures)
		    	{
		    	    //std::cout << "dmdtextures[" << texdatnum << "]:\n";
		    	    _io::unpack(&texdat->version, istr, 1, true);
		    	    //std::cout << " ->version = " << texdat->version << ";\n";
		    	    //texdat->use_rgb2intensity = false;
		    	    for (_ddfwalker_t ddf2(istr); ddf2.adv();)
		    	    {
		    	    	if (ddf2.key == ddfkey_t("TEXIMG"))
		    	    	{
		    	    		uint32_t tii; ddf2.unpack(&tii, 1, true);
		    	    		//std::cout << " ->teximg = dmdteximages[" << tii << "];\n";
		    	            texdat->teximg = this->dmdteximages[tii];
		    	    	}
                        else if (ddf2.key == ddfkey_t("TEXSRC")){
                            ddf2.unpack<texsrc, uint32_t>(&texdat->texsource, 1, true);
                        }
		    	    	else if (ddf2.key == ddfkey_t("BLENDT"))
		    	    	{
		    	    		ddf2.unpack<texblend, uint8_t>(&texdat->blendtype, 1, true);
		    	    		//std::cout << " ->blendtype = " << int(texdat->blendtype) << ";\n";
		    	    	}
		    	    	else if (ddf2.key == ddfkey_t("UVMAP"))
		    	    	{
		    	    		ddf2.unpack(&texdat->uvmap_index, 1, true);
		    	    		//std::cout << " ->uvmap_index = " << texdat->uvmap_index << ";\n";
		    	    	}
		    	    	else if (ddf2.key == ddfkey_t("MAPTYPE"))
		    	    	{
		    	    		ddf2.unpack<texmap, uint16_t>(&texdat->maptype, 1, true);
		    	    		//std::cout << " ->maptype = " << int(texdat->maptype) << ";\n";
		    	    	}
		    	    	else if (ddf2.key == ddfkey_t("BMSPACE")){
                            ddf2.unpack<bumpmapspace, uint8_t>(&texdat->bumpspace, 1);
		    	    	}
		    	    	else if (ddf2.key == ddfkey_t("RGB2INT")){
		    	    	    texdat->use_rgb2intensity = true;
                            ddf2.unpack(&(texdat->rgb2intensity_color[0]), 3, true);
		    	    	}
		    	    	else if (ddf2.key == ddfkey_t("ADC")){texdat->affects_diffuse_color.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("ASC")){texdat->affects_specular_color.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("AAC")){texdat->affects_ambient_color.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("ADA")){texdat->affects_diffuse_alpha.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("ASA")){texdat->affects_specular_alpha.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("ADI")){texdat->affects_diffuse_intensity.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("ASI")){texdat->affects_specular_intensity.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("ASH")){texdat->affects_specular_hardness.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("ATO")){texdat->affects_texcoord_offset.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("ANV")){texdat->affects_normal_vector.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("AEA")){texdat->affects_emit_amount.readf(istr);}
		    	    	else if (ddf2.key == ddfkey_t("TEXFX")) {
                            texdat->texfx.read(istr, strlist);
                        }
		    	    	else
		    	    	{
		    	    		std::cout << "dmdtextures[" << texdatnum << "]: skip key <" << ddf2.key << "> with datalen=" << ddf2.datalen << "\n";
		    	    	    ddf2.skip2next();
		    	    	}

		    	    }
		    		texdatnum++;
		    	}

		    	size_t teximgnum = 0;
		    	uint32_t total_num_images = 0, numlvls = 1;
		    	for (DMD_TexImage*& teximg : this->dmdteximages)
		    	{
		    	    //std::cout << "dmdteximages[" << teximgnum << "]:\n";
		    	    //uint8_t origin = 2;
		    	    teximg->version = 0;
		    	    numlvls = 1;
		    	    if (this->dmdversion >= 1)
		    	    {
		    	    	_io::unpack(&teximg->version, istr, 1, true);
		    	    	_io::unpack(&total_num_images, istr, 1, true);
		    	    	_io::unpack(&numlvls, istr, 1, true);
		    	    }
		    	    else
		    	    {
		    	    	total_num_images = 1;
		    	    }

		    	    teximg->images.resize(numlvls);
		    	    size_t lvlnum = 0;
		    	    for (teximglvl_t& level : teximg->images)
		    	    {
		    	        //std::cout << " ->images[" << lvlnum << "]:\n";
		    	    	//origin = 2;
		    	    	level.origin = texorigin::bl;
		    	    	if (this->dmdversion >= 1)
		    	    	{
		    	    	    for (_ddfwalker_t ddf2(istr); ddf2.adv();)
		    	    	    {
		    	    	    	if (ddf2.key == ddfkey_t("ORIGIN"))
		    	    	    	{
		    	    	    		ddf2.unpack<texorigin, uint8_t>(&level.origin, 1, true);
		    	    	    	}
		    	    	    	else
		    	    	    	{
		    	    	    		ddf2.skip2next();
		    	    	    	}
		    	    	    }
		    	    	}
		    	    	level.read_rim(istr);
		    	    	//std::cout << "  ->width = " << level.width << ";\n";
		    	    	//std::cout << "  ->height = " << level.height << ";\n";
		    	    	//std::cout << "  ->cpp = " << level.cpp << ";\n";
		    	    	//std::cout << "  ->origin = " << int(level.origin) << ";\n";
		    	    	lvlnum++;
		    	    }
		    		teximgnum++;
		    	}
		    	//std::cout << "The DMD file has been successfully loaded!\n";
		    }
		    void load(const std::string filename)
		    {
		    	std::fstream ifs(filename, std::ios::in | std::ios::binary);
		    	this->load(ifs);
		    	ifs.close();
		    }
		    uint32_t calc_stringlist(std::vector<std::string>& out_strlist) {
		    	#define _is_in(_s) (std::find(out_strlist.begin(), out_strlist.end(), _s) != out_strlist.end())

                for (auto*& node : this->dmdnodes) {
                	if (!_is_in(node->name)){
                		out_strlist.push_back(node->name);
                	}
                }

		    	#undef _is_in
		    	return uint32_t(out_strlist.size());
		    }
		    size_t save(std::ostream& ostr) {
				this->ensuresAllDataInLists();
		    	const uint32_t _dmdversion = 1;
		    	std::stringstream errss;
		    	auto startpos = ostr.tellp();

		    	binio::pack(ostr, _DMD_FILE_MAGIC+0, 8);
		    	binio::pack(ostr, &_dmdversion, 1, true);

		    	std::vector<std::string> strlist = {};

		    	uint32_t nstrs = this->calc_stringlist(strlist);
		    	uint32_t nnodes = this->dmdnodes.size() - 1;
		    	uint32_t nmeshes = this->dmdmeshes.size();
		    	uint32_t nsubmdls = this->dmdsubmodels.size();
		    	uint32_t nmats = this->dmdmaterials.size();
		    	uint32_t ntexdats = this->dmdtextures.size();
		    	uint32_t nteximgs = this->dmdteximages.size();

		    	_ddfwriter_t _hdrw(ostr);

		    	_hdrw.newkey(ddfkey_t("NSTRS")); _hdrw.pack(&nstrs, 1, true);
		    	_hdrw.newkey(ddfkey_t("NNODES")); _hdrw.pack(&nnodes, 1, true);
		    	_hdrw.newkey(ddfkey_t("NMESHES")); _hdrw.pack(&nmeshes, 1, true);
		    	_hdrw.newkey(ddfkey_t("NSMDLS")); _hdrw.pack(&nsubmdls, 1, true);
		    	_hdrw.newkey(ddfkey_t("NMATS")); _hdrw.pack(&nmats, 1, true);
		    	_hdrw.newkey(ddfkey_t("NTEXDATS")); _hdrw.pack(&ntexdats, 1, true);
		    	_hdrw.newkey(ddfkey_t("NTEXIMGS")); _hdrw.pack(&nteximgs, 1, true);
		    	_hdrw.newkey(ddfkey_t("XDATSIZE")); _hdrw.pack("\0\0\0\0", 4);

		    	_hdrw.terminate();

		    	for (std::string& str : strlist) {
		    	    uint16_t strl = str.size();
		    		binio::pack(ostr, &strl, 1, true);
		    		ostr.write(str.data(), str.size());
		    	}
		    	uint32_t nodeindex = 0;
		    	for (auto nitr = this->dmdnodes.begin()+1; nitr != this->dmdnodes.end(); nitr++, nodeindex++) {
		    	    DMD_Node*& node = *nitr;
		    		//const uint32_t nodever = 0;
		    		binio::pack(ostr, &node->version, 1, true);
		    		_ddfwriter_t _nodew(ostr);
		    		_nodew.newkey(ddfkey_t("PNINDEX"));
		    		    //auto fit = std::find(this->dmdnodes.begin(), this->dmdnodes.end(), node->parent);
		    		    //auto pni = uint32_t(fit - this->dmdnodes.begin());
		    		    uint32_t pni = _findfirstidx(this->dmdnodes, node->parent);
		    		    _nodew.pack(&pni, 1, true);
		    		_nodew.newkey(ddfkey_t("NINDEX"));
		    		    uint32_t idx = _findfirstidx(strlist, node->name);
		    		    _nodew.pack(&idx, 1, true);
		    		_nodew.newkey(ddfkey_t("NTYPE"));
		    		    _nodew.pack(node->nodetype+0, 4);
		    		_nodew.newkey(ddfkey_t("LOCPOS"));
		    		    _nodew.pack(node->local_position.data(), 3, true);
		    		_nodew.newkey(ddfkey_t("LOCSCL"));
		    		    _nodew.pack(node->local_scale.data(), 3, true);
		    		_nodew.newkey(ddfkey_t("LOCORN"));
		    		    _nodew.pack(node->local_orientation[0].data(), 3, true);
		    		    _nodew.pack(node->local_orientation[1].data(), 3, true);
		    		    _nodew.pack(node->local_orientation[2].data(), 3, true);
		    		_nodew.newkey(ddfkey_t("SUBMDL"));
		    		    uint32_t smi = 0;
		    		    if (node->submdl != nullptr){
							smi = _findfirstidx(this->dmdsubmodels, node->submdl);
						}
		    		    _nodew.pack(&smi, 1, true);
		    		_nodew.newkey(ddfkey_t("VISIBLE"));
		    		    ostr.put(int(node->visible));
                    _nodew.newkey(ddfkey_t("PSHAPE"));
                        _nodew.pack((uint16_t*)&(node->physics_shape), 1, true);
                    _nodew.newkey(ddfkey_t("COLLMASK"));
                        _nodew.pack(&(node->collision_mask), 1, true);
		    		_nodew.terminate();
		    	}
		    	uint32_t meshindex = 0;
		    	for (DMD_Mesh*& mesh : this->dmdmeshes) {
					//const uint32_t meshver = 0;
					uint32_t
					    nvertices = mesh->vertices.size(),
					    nuvmaps = mesh->uvmaps.size(),
					    nfaces = mesh->indices.size() / 3U
					;
					uint8_t uvmode = 0;

					binio::pack(ostr, &mesh->version, 1, true);
					binio::pack(ostr, &nvertices, 1, true);
					binio::pack(ostr, &nuvmaps, 1, true);
					binio::pack(ostr, &uvmode, 1);
					binio::pack(ostr, &nfaces, 1, true);

					_ddfwriter_t meshw(ostr);
					meshw.newkey(ddfkey_t("VERTS"));
					    meshw.pack(mesh->vertices[0].data(), nvertices * 3, true);
					meshw.newkey(ddfkey_t("COLORS"));
					    meshw.pack(mesh->vertex_colors[0].data(), nvertices * 3, true);
					meshw.newkey(ddfkey_t("NORMALS"));
					    meshw.pack(mesh->normals[0].data(), nvertices * 3, true);
					meshw.newkey(ddfkey_t("UVMAPS"));
					    for (auto& uvmap : mesh->uvmaps) {
							meshw.pack(uvmap[0].data(), nvertices * 2, true);
						}
					meshw.newkey(ddfkey_t("FACES"));
					    meshw.pack(mesh->indices.data(), mesh->indices.size(), true);
					meshw.newkey(ddfkey_t("MATINDEX"));
					    uint32_t matidx = 0;
					    if (mesh->material != nullptr){
							matidx = _findfirstidx(this->dmdmaterials, mesh->material);
						}
						meshw.pack(&matidx, 1, true);
					meshw.terminate();
					meshindex++;
				}
				uint32_t submdlidx = 0;
				for (DMD_SubModel*& submdl : this->dmdsubmodels) {
					binio::pack(ostr, &submdl->version, 1, true);
					_ddfwriter_t submdlw(ostr);
					submdlw.newkey(ddfkey_t("MESHES"));
					    uint32_t nmeshes = submdl->meshes.size();
					    submdlw.pack(&nmeshes, 1, true);
					    for (DMD_Mesh*& submesh : submdl->meshes) {
							uint32_t smmi = _findfirstidx(this->dmdmeshes, submesh);
							submdlw.pack(&smmi, 1, true);
						}
					submdlw.terminate();
					submdlidx++;
				}
				uint32_t materialidx = 0;
				for (DMD_Material*& material : this->dmdmaterials){
					binio::pack(ostr, &material->version, 1, true);
					_ddfwriter_t materialw(ostr);
					for (uint32_t tsi = 0; tsi < 8; tsi++) {
						materialw.newkey(ddfkey_t("TEXSLOT"));
						const auto& mts = material->texture_slots[tsi];
						materialw.pack(&tsi, 1, true);
						uint8_t in_use(mts.use);
						materialw.pack(&in_use, 1);
						uint32_t tdi = 0;
						if (mts.use) {
							tdi = uint32_t(_findfirstidx(this->dmdtextures, mts.texdata));
						}
						materialw.pack(&tdi, 1, true);
				    }
				    materialw.newkey(ddfkey_t("SHDLESS"));
				        uint8_t shdless(material->shadeless);
				        materialw.pack(&shdless, 1);
				    #define _packvec(_k, _field) materialw.newkey(ddfkey_t(_k)); materialw.pack(material->_field.data(), material->_field.size(), true)
				    #define _pack1f(_k, _field) materialw.newkey(ddfkey_t(_k)); materialw.pack(&(material->_field), 1, true)

				    _packvec("BDC", base_diffuse_color);
				    _packvec("BSC", base_specular_color);
				    _packvec("BAC", base_ambient_color);

				    _pack1f("BDA", base_diffuse_alpha);
				    _pack1f("BSA", base_specular_alpha);
				    _pack1f("BDI", base_diffuse_intensity);
				    _pack1f("BSI", base_specular_intensity);
				    _pack1f("BSH", base_specular_hardness);
				    _pack1f("BEA", base_emit_amount);

				    #undef _packvec
				    #undef _pack1f
				    uint8_t facecull = uint8_t(material->facecull);
                    materialw.newkey(ddfkey_t("FACECULL"));
                        materialw.pack(&facecull, 1);
				    uint8_t blendt = uint8_t(material->blendtype);
				    materialw.newkey(ddfkey_t("BLENDT"));
				        materialw.pack(&blendt, 1);
					materialw.terminate();

					materialidx++;
				}
				uint32_t texdatidx = 0;
				for (DMD_TexData*& texdata : this->dmdtextures) {
					binio::pack(ostr, &texdata->version, 1, true);
					_ddfwriter_t texdataw(ostr);

					texdataw.newkey(ddfkey_t("TEXIMG"));
					    uint32_t tii(_findfirstidx(this->dmdteximages, texdata->teximg));
					    texdataw.pack(&tii, 1, true);
					texdataw.newkey(ddfkey_t("BLENDT"));
					    uint8_t tbt = uint8_t(texdata->blendtype);
					    texdataw.pack(&tbt, 1);
					texdataw.newkey(ddfkey_t("UVMAP"));
					    texdataw.pack(&texdata->uvmap_index, 1, true);
					texdataw.newkey(ddfkey_t("MAPTYPE"));
					    uint16_t tmt = uint16_t(texdata->maptype);
					    texdataw.pack(&tmt, 1, true);
					texdataw.newkey(ddfkey_t("TEXSRC"));
                        uint32_t tsrc = uint32_t(texdata->texsource);
                        texdataw.pack(&tsrc, 1, true);
                    if (texdata->use_rgb2intensity){
                        texdataw.newkey(ddfkey_t("RGB2INT"));
                            texdataw.pack(&(texdata->rgb2intensity_color[0]), 3, true);
                    }

					#define _packta(_k, _f) texdataw.newkey(ddfkey_t(_k)); texdata->_f.writef(ostr)

					_packta("ADC", affects_diffuse_color);
					_packta("ASC", affects_specular_color);
					_packta("AAC", affects_ambient_color);
					_packta("ADA", affects_diffuse_alpha);
					_packta("ASA", affects_specular_alpha);
					_packta("ADI", affects_diffuse_intensity);
					_packta("ASI", affects_specular_intensity);
					_packta("ASH", affects_specular_hardness);
					_packta("ATO", affects_texcoord_offset);
					_packta("ANV", affects_normal_vector);
					_packta("AEA", affects_emit_amount);

					#undef _packta

					texdataw.terminate();
					texdatidx++;
				}
				uint32_t teximgidx = 0;
				uint32_t totalcount = 0;
				for (DMD_TexImage*& teximg : this->dmdteximages) {
					uint32_t teximgver = 0, numlvls = teximg->images.size();
					binio::pack(ostr, &teximgver, 1, true);
					binio::pack(ostr, &totalcount, 1, true);
					binio::pack(ostr, &numlvls, 1, true);
					totalcount += numlvls;
					uint32_t lvlidx = 0;
					for (teximglvl_t& lvl : teximg->images) {
					    _ddfwriter_t teximgw(ostr);
					    teximgw.newkey(ddfkey_t("ORIGIN"));
					        uint8_t origin = uint8_t(lvl.origin);
					        teximgw.pack(&origin, 1);
					    teximgw.terminate();
					    lvl.write_rim(ostr);
					    lvlidx++;
					}
					teximgidx++;
				}
		    	auto endpos = ostr.tellp();
		    	return size_t(endpos) - size_t(startpos);
		    }
		    size_t save(const std::string filename) {
		    	std::fstream ofs(filename, std::ios::out | std::ios::binary);
		    	const auto n = this->save(ofs);
		    	ofs.close();
		    	return n;
		    }
		    DMDFile() {}
		    ~DMDFile() {this->close();}
		};
}};
#endif
#if (defined(DJUTIL_NEEDS_virfs) && !defined(__DJUTIL_H_virfs_LOADED))
#define __DJUTIL_H_virfs_LOADED
namespace djutil{namespace virfs{

    class VirtualFSSourceBase;
	class VirtualFS;

	using vsrclist_t = djutil::containers::vectorlist<VirtualFSSourceBase*>;
	using fspath_t = std::u32string;
	using fspathlist_t = djutil::containers::vectorlist<fspath_t>;
	struct fspathpair_t {
		fspath_t fullname, relname;
		bool operator==(const fspathpair_t& other) const {
			return this->fullname == other.fullname;
		}
	};
	using fsdirlist_t = djutil::containers::vectorlist<fspathpair_t>;
    enum struct ent_type : int {
    	nonexistent = 0,
    	file = 1,
    	dir = 2
    };

    enum struct src_type : int {
    	srcbase = 0,
    	dirsrc = 1,
    	qpaksrc = 2,
    	pk2src = 3,
    	rpksrc = 4
    };

    struct read_member_data {
    	std::istream* hnd = nullptr;
    	std::streampos mstart, mend;
    	std::streamsize msize;
    	VirtualFSSourceBase* src = nullptr;
    	size_t mstrid = 0;
    };

    class vfs_isubstreambuf : public std::streambuf {
    	private:
    	    std::streambuf* superbuf = nullptr;
    	    std::streampos startpos = 0, endpos = 0, curofs = 0;
    	    std::streamsize subsize = 0;
    	    char _buf1;
    	    void _reseek() {
				const std::streampos
				    mypos = this->startpos + this->curofs,
				    superpos = this->superbuf->pubseekoff(0,std::ios::cur,std::ios::in)
				;
				if (mypos != superpos){this->superbuf->pubseekpos(mypos, std::ios::in);}
    	    }
            void _private_destroy();
    	protected:
    	    virtual int underflow() {
    	        if (this->curofs >= this->subsize){return traits_type::eof();}
    	        this->_reseek();
    	    	traits_type::int_type ri = this->superbuf->sbumpc();
    	    	if (ri == traits_type::eof())
    	    	{
    	    		throw std::runtime_error("unexpected EOF from source buffer!");
    	    	}
    	    	this->_buf1 = traits_type::to_char_type(ri);
    	    	this->setg(&this->_buf1, &this->_buf1, (&this->_buf1)+1);
    	    	this->curofs = this->curofs + decltype(this->curofs)(1);
    	    	return ri;
    	    }
    	    virtual int sync() {
    	    	this->_reseek();
    	    	return 0;
    	    }
    	    virtual std::streamsize showmanyc() {
    	    	std::streamsize diff = this->subsize - std::streamsize(this->curofs);
    	    	return (diff == std::streamsize(0) ? std::streamsize(-1) : diff);
    	    }

    	    virtual std::streamsize xsgetn(char* buf, std::streamsize num) {
				this->_reseek();
				std::streamsize clampednum = std::min(num, std::streamsize(this->endpos - this->curofs));
				std::streamsize gcount = this->superbuf->sgetn(buf, clampednum);
				this->curofs += std::streampos(gcount);
				return gcount;
			}

    	    virtual pos_type seekoff(off_type ofs, std::ios_base::seekdir dir, std::ios_base::openmode which=std::ios_base::in) {
    	    	switch (dir)
    	    	{
    	    		case std::ios_base::beg: {
    	    		    this->curofs = ofs;
    	    			break;
    	    		}
    	    		case std::ios_base::cur: {
    	    			this->curofs += ofs;
    	    			break;
    	    		}
    	    		case std::ios_base::end: {
    	    			this->curofs = std::streampos(this->subsize) + ofs;
    	    			break;
    	    		}
    	    		default: {}
    	    	}
    	    	if (this->curofs > this->subsize){this->curofs = this->subsize;}
    	    	if (this->curofs < 0){this->curofs = 0;}
    	    	return pos_type(this->curofs);
    	    }
    	    virtual pos_type seekpos(pos_type pos, std::ios_base::openmode which = std::ios_base::in) {
    	    	return this->seekoff(pos, std::ios_base::beg, which);
    	    }

    	public:
            //uint32_t member_id = -1;
            void* userdata[32] = {};
            fspath_t member_name = U"";
            VirtualFSSourceBase* vfssrc = nullptr;

    	    vfs_isubstreambuf() {}
    	    vfs_isubstreambuf(std::streambuf* _sbuf, const std::streampos _pos=0, const std::streamsize _size=0) :
    	        superbuf(_sbuf),
    	        startpos(_pos), endpos(_pos+std::streampos(_size)),
    	        subsize(_size)
    	    {
    	        char *buf = &this->_buf1, *bufend = buf+1;
    	        this->setg(buf, bufend, bufend);
    	    }
            ~vfs_isubstreambuf(){this->_private_destroy();}
    };


	class VirtualFSSourceBase {
	    private:
	        VirtualFS* _vfs = nullptr;
	    protected:
	        virtual void _on_deletion() {}
		public:
		    VirtualFS* const& virfs = _vfs;
		    virtual ent_type getEntityType(const fspath_t& entname) {
		    	return ent_type::nonexistent;
		    }

		    virtual bool getDirectoryListing(fsdirlist_t& out_dirs, fsdirlist_t& out_files, const fspath_t& entpath) {
		    	return false;
		    }

		    virtual src_type getSourceType() {return src_type::srcbase;}

		    virtual bool getFileMemberReadHandle(vfs_isubstreambuf& istr, const fspath_t& ent) {
		    	return false;
		    }
		    virtual bool copyFileMemberToSStream(std::stringstream& sstr, const fspath_t& ent) {
				vfs_isubstreambuf sb;
				if (!this->getFileMemberReadHandle(sb, ent)){return false;}
				//sstr << sb;
				while (true) {
					int c = sb.sbumpc();
					if (c == EOF){break;}
					sstr.put(c);
				}
				return true;
			}
            virtual void onFileMemberReadHandleDelete(vfs_isubstreambuf& hnd) {}
		    //VirtualFSSourceBase(VirtualFS* _parent_vfs=nullptr) : _vfs(_parent_vfs) {}
		    VirtualFSSourceBase() {}
		    VirtualFSSourceBase(VirtualFS* _parent_vfs);
		    ~VirtualFSSourceBase();
	};

    void vfs_isubstreambuf::_private_destroy() {
        if (this->vfssrc != nullptr){
            this->vfssrc->onFileMemberReadHandleDelete(*this);
        }
    }

	class VirtualFS {
	    private:
	        ;
		public:
		    vsrclist_t sources = {};

		    ent_type getEntityType(const fspath_t& entpath, VirtualFSSourceBase*& findsrc) {
		        for (auto*& src : this->sources)
		        {
		        	ent_type et = src->getEntityType(entpath);
		        	if (et != ent_type::nonexistent)
		        	{
		        	    findsrc = src;
		        		return et;
		        	}
		        }
		    	return ent_type::nonexistent;
		    }
		    ent_type getEntityType(const fspath_t& entpath) {
		    	VirtualFSSourceBase* findsrc = nullptr;
		    	return this->getEntityType(entpath, findsrc);
		    }

		    void listdir(fsdirlist_t& dirs, fsdirlist_t& files, const fspath_t& entpath) {
		        containers::dictionary<fspath_t, bool> _filenames_added = {}, _dirnames_added = {};
		    	dirs.clear();
		    	files.clear();
		    	fspath_t fsdir = entpath;
		    	if (fsdir.size() > 0 && (fsdir.back() != U'/')) {
		    		fsdir += U"/";
		    	}
		    	for (VirtualFSSourceBase*& src : this->sources) {
		    	    //if (src->getEntityType(fsdir) != ent_type::dir){continue;}
		    		fsdirlist_t rdirs = {}, rfiles = {};
		    		if (src->getDirectoryListing(rdirs, rfiles, fsdir)) {
		    			for (auto& rdir : rdirs) {
                            if (rdir.fullname.back() != U'/'){rdir.fullname.push_back(U'/');}
		    				if (_dirnames_added.count(rdir.fullname) == 0 || !_dirnames_added[rdir.fullname]) {
		    				    _dirnames_added[rdir.fullname] = true;
		    					dirs.push_back(rdir);
		    				}
		    			}
		    			for (auto& rfile : rfiles) {
		    				if (_filenames_added.count(rfile.fullname) == 0 || !_filenames_added[rfile.fullname]) {
		    				    _filenames_added[rfile.fullname] = true;
		    					files.push_back(rfile);
		    				}
		    			}
		    		}
		    	}
		    }
		    void open(vfs_isubstreambuf& istr, const fspath_t& fileent) {
		    	for (auto*& src : this->sources) {
		    		if (src->getFileMemberReadHandle(istr, fileent)){return;}
		    	}
		    	//std::string fileent_u8; ezstr::wide2mb<char, char32_t>(fileent_u8, fileent);
		    	throw std::runtime_error("can't open ent for reading: "+ezstr::utf32_to_utf8(fileent));
		    }
		    void open(std::stringstream& sstr, const fspath_t& fileent) {
				for (auto*& src : this->sources) {
					if (src->copyFileMemberToSStream(sstr, fileent)){return;}
				}
				throw std::runtime_error("can't open ent for copying: "+ezstr::utf32_to_utf8(fileent));
			}
		    VirtualFS() {}
		    ~VirtualFS();
	};
	VirtualFSSourceBase::VirtualFSSourceBase(VirtualFS* _parent_vfs) {
		this->_vfs = _parent_vfs;
		this->_vfs->sources.push_back(this);
	}
	VirtualFSSourceBase::~VirtualFSSourceBase() {
	    this->_on_deletion();
		if (this->_vfs != nullptr)
		{
			this->_vfs->sources.eraseAllOf(this);
			this->_vfs = nullptr;
		}
	}
	VirtualFS::~VirtualFS() {
		while (this->sources.size() > 0)
		{
			delete this->sources.back();
		}
	}

	class DirectorySource : public VirtualFSSourceBase {
	    using VirtualFSSourceBase::VirtualFSSourceBase;
        private:
            uint32_t _cur_memberid = 1;
            fspath_t _rootdir = U"";

            void _ensure_rootdir_endsep() {
                if (
                    this->_rootdir.size() == 0 || (
                        this->_rootdir.back() != U'\\' &&
                        this->_rootdir.back() != U'/'
                    )
                ){
                    this->_rootdir.push_back(char32_t(djutil::pathman::_DEFAULT_SYNTAX));
                }
            }
        public:
            const fspath_t& rootdir = _rootdir;

            fspath_t _getRealPath(const fspath_t& fsp) {
                fspath_t out = this->_rootdir;
                //out += U".";
                const char32_t sep = char32_t(djutil::pathman::_DEFAULT_SYNTAX);
                fspath_t _head, _tail;
                djutil::ezstr::partition<char32_t>(_head, _tail, fsp, U"/");
                std::vector<fspath_t> tokens; djutil::ezstr::split<char32_t>(tokens, _tail, U"/");
                for (auto& token : tokens) {
                    if (token.size() == 0){continue;}
                    out += token;
                    out.push_back(sep);
                }
                if (fsp.back() != U'/' && out.back() == sep){out.pop_back();}
                //if (fsp.back() == U'/' && out.back() != sep){out.push_back(sep);}
                std::cout << "out = U" << std::quoted(djutil::ezstr::utf32_to_utf8(out)) << '\n';
                return out;
            }

            virtual bool getDirectoryListing(fsdirlist_t& out_dirs, fsdirlist_t& out_files, const fspath_t& path) {
                fspath_t realpath = this->_getRealPath(path);
                fspath_t sep = U" ";
                sep[0] = char32_t(djutil::pathman::_DEFAULT_SYNTAX);
                if (!djutil::ezstr::endswith<char32_t>(realpath, sep)) {
                    realpath += sep;
                }
                const std::string path_u8 = djutil::ezstr::utf32_to_utf8(realpath);
                std::cout << "dirsrc getdirlist path_u8 = " << std::quoted(path_u8) << '\n';
                std::vector<std::string> listing;
                if (!djutil::pathman::listdir(listing, path_u8)){return false;}
                for (auto& entu8 : listing) {
                    if (entu8 == "." || entu8 == ".."){continue;}
                    const std::string full_u8 = path_u8 + entu8;
                    fspathpair_t pp = {};
                    pp.relname = djutil::ezstr::utf8_to_utf32(entu8);
                    pp.fullname = path;
                    if (pp.fullname.back() != U'/'){pp.fullname += U"/";}
                    pp.fullname += pp.relname;
                    if (djutil::pathman::is_dir(full_u8)){
                        if (pp.fullname.back() != U'/'){pp.fullname += U"/";}
                        out_dirs.push_back(pp);
                    }
                    else if (djutil::pathman::is_file(full_u8)){
                        out_files.push_back(pp);
                    }
                }
                return true;
            }

            virtual ent_type getEntityType(const fspath_t& fsp) {
                const std::string realpath = djutil::ezstr::utf32_to_utf8(this->_getRealPath(fsp));
                if (djutil::pathman::is_dir(realpath)){return ent_type::dir;}
                else if (djutil::pathman::is_file(realpath)){return ent_type::file;}
                return ent_type::nonexistent;
            }
            struct _member_handle_data {
                fspath_t entname;
                uint32_t opencount;
                djutil::ufpio::ufstream* handle = nullptr;
                size_t filesize;
            };
            std::unordered_map<fspath_t, _member_handle_data> fspath_to_hnds;
            std::unordered_map<fspath_t, std::ifstream*> _hndmap;
            virtual bool getFileMemberReadHandle(vfs_isubstreambuf& sb, const fspath_t& ent) {
                if (this->getEntityType(ent) != ent_type::file){return false;}
                fspath_t realpath = this->_getRealPath(ent);
                std::cout << "realpath = " << std::quoted(djutil::ezstr::utf32_to_utf8(realpath)) << "\n";
                /*
                if (this->fspath_to_hnds.count(ent) == 0) {
                    _member_handle_data _mhnd = {};
                    _mhnd.entname = ent;
                    //_mhnd.member_id = (this->_cur_memberid);
                    _mhnd.opencount = 0;
                    _mhnd.handle = new djutil::ufpio::ufstream();
                    _mhnd.handle->open(realpath, std::ios::in | std::ios::binary | std::ios::ate);
                    _mhnd.filesize = _mhnd.handle->tellg();
                    _mhnd.handle->seekg(0);
                    this->fspath_to_hnds[ent] = _mhnd;
                    //this->memberid_to_fspath[_mhnd.member_id] = ent;
                    //this->_cur_memberid++;
                }
                auto& mhnd = this->fspath_to_hnds[ent];
                sb = vfs_isubstreambuf(mhnd.handle->rdbuf(), 0, mhnd.filesize);
                sb.member_name = ent;
                mhnd.opencount++;
                */
                auto* fhnd = new djutil::ufpio::ufstream();
                fhnd->open(realpath, std::ios::in | std::ios::binary | std::ios::ate);
                auto filesize = fhnd->tellg(); fhnd->seekg(0);
                sb = vfs_isubstreambuf(fhnd->rdbuf(), 0, filesize);
                sb.userdata[0] = fhnd;
                return true;
            }
            virtual bool copyFileMemberToSStream(std::stringstream& sstr, const fspath_t& ent) {
                if (this->getEntityType(ent) != ent_type::file){return false;}
                djutil::ufpio::ufstream m;
                m.open(this->_getRealPath(ent), std::ios::in | std::ios::binary);
                sstr << m.rdbuf();
                m.close();
                return true;
            }
            virtual void onFileMemberReadHandleDelete(vfs_isubstreambuf& hnd) {
                /*
                if (this->fspath_to_hnds.count(hnd.member_name) == 0){return;}
                auto& mhnd = this->fspath_to_hnds[hnd.member_name];
                mhnd.opencount--;
                if (mhnd.opencount == 0){
                    mhnd.handle->close();
                    this->fspath_to_hnds.erase(hnd.member_name);
                    delete mhnd.handle;
                    mhnd.handle = nullptr;
                }
                hnd.member_name = U"";
                */
                if (hnd.userdata[0] != nullptr){
                    auto* fhnd = reinterpret_cast<djutil::ufpio::ufstream*>(hnd.userdata[0]);
                    fhnd->close();
                    delete fhnd;
                    hnd.userdata[0] = nullptr;
                }
            }

            virtual src_type getSourceType() {return src_type::dirsrc;}

            virtual void _on_deletion() {
                /*
                while (this->fspath_to_hnds.size() > 0){
                    auto& p = *(this->fspath_to_hnds.begin());
                    //p.second.handle->close();
                    delete p.second.handle;
                    p.second.handle = nullptr;
                    this->fspath_to_hnds.erase(p.first);
                }
                this->fspath_to_hnds.clear();
                */
            }

            DirectorySource(VirtualFS* _parent_vfs, const fspath_t& root) :
                VirtualFSSourceBase(_parent_vfs),
                _rootdir(root)
            {
                this->_ensure_rootdir_endsep();
            }
	};

	typedef size_t rpkpos_t;
	enum class _rpknodetype : uint8_t {
		dir = 0,
		file = 1
	};
	enum class _rpkcomprtype : uint32_t {
		none = 0
	};
	struct _rpksrc_nodedata {
		fspath_t name = U"", fullname = U"/";
		_rpknodetype nodetype = _rpknodetype::dir;
		char _pad1[3];
		uint32_t
		    nodeindex = 0,
		    parentnodeindex = 0,
		    fullnameindex = 0,
		    tot_num_childnodes = 0,
		    num_subdirs = 0,
		    num_files = 0
		;
		rpkpos_t offset = 0, size = 0;

		_rpkcomprtype compression = _rpkcomprtype::none;
		rpkpos_t uncompressed_size;
	};
	typedef containers::treenode<_rpksrc_nodedata*> _rpksrc_fsnode;

	class ResourcePackSource : public VirtualFSSourceBase {
		using VirtualFSSourceBase::VirtualFSSourceBase;
		private:
		    ufpio::ufstream _arc_ifstr;
		    std::istream* arc_istr = nullptr;
		    containers::vectorlist<fspath_t> stringlist = {};
		    containers::vectorlist<_rpksrc_fsnode*> fsnodes = {};
		    containers::vectorlist<_rpksrc_nodedata> nodedatas = {};
		    containers::dictionary<fspath_t, _rpksrc_fsnode*> dirnodes = {}, filenodes = {};

		    _rpksrc_fsnode* root = nullptr;

		    _rpksrc_fsnode* init_node(const uint32_t idx=0) {
				if (fsnodes[idx] != nullptr){return fsnodes[idx];}
				_rpksrc_fsnode*& nn = fsnodes[idx];

				nn = new _rpksrc_fsnode();
				nn->data = &(this->nodedatas[idx]);
				nn->data->nodeindex = idx;

				if (idx != 0) {
					nn->data->fullname = this->stringlist[nn->data->fullnameindex];
					fspath_t dummy;
					pathman::split(dummy, nn->data->name, nn->data->fullname, pathman::pathsyntax::posix);
					if (ezstr::endswith(nn->data->name, U"/")){nn->data->name.pop_back();}
					std::cout << "name = U" << std::quoted(ezstr::utf32_to_utf8(nn->data->name)) << ", fullname = U" << ezstr::utf32_to_utf8(nn->data->fullname) << "\n";
				}
				else {
					nn->data->fullname = U"/";
					nn->data->name = U"";

					this->root = nn;
				}

				switch (nn->data->nodetype) {
					case _rpknodetype::file: {
						this->filenodes[nn->data->fullname] = nn;
						break;
					}
					default: {
						this->dirnodes[nn->data->fullname] = nn;
						break;
				    }
				}
				return nn;
			}

			void _entree_fsnodes() {
				for (auto it = this->fsnodes.begin()+1; it != this->fsnodes.end(); it++) {
					_rpksrc_fsnode *cnode = *it, *pnode = this->fsnodes[cnode->data->parentnodeindex];
					pnode->addchild(cnode);
				}
			}

		protected:
		    void _read_archive() {
				std::istream& in = *this->arc_istr;
				#define _unpackLE(_p, _n) binio::unpack(_p, in, _n, true)
				#define _read_pos_t(_p, _n) (hdr.pos_t == 1 ? binio::unpack<rpkpos_t, uint64_t>(_p, in, _n, true) : binio::unpack<rpkpos_t, uint32_t>(_p, in, _n, true))
				auto startpos = in.tellg();
				struct {
					char file_id[4];
					uint32_t version;
					uint8_t pos_t;
					rpkpos_t strlist_offs, strlist_size;
					uint32_t strlist_numents;
					uint8_t strlist_encoding;
					uint32_t authors, infotxt;
					rpkpos_t nodedata_offs, nodedata_size;
					uint32_t total_num_nodes;
					rpkpos_t ena_offs, ena_size, fcb_offs, fcb_size;
				} hdr;
				_unpackLE(hdr.file_id, 4);
				if (memcmp(hdr.file_id, "RPK\0", 4)){throw std::runtime_error("Not an .rpk!");}
				_unpackLE(&hdr.version, 1);
				_unpackLE(&hdr.pos_t, 1);
				in.seekg(5, std::ios::cur);
				_read_pos_t(&hdr.strlist_offs, 2);
				_unpackLE(&hdr.strlist_numents, 1);
				_unpackLE(&hdr.strlist_encoding, 1);
				in.seekg(3, std::ios::cur);
				_unpackLE(&hdr.authors, 2);
				_read_pos_t(&hdr.nodedata_offs, 2);
				_unpackLE(&hdr.total_num_nodes, 1);
				_read_pos_t(&hdr.ena_offs, 4);
				auto hdrendpos = in.tellg();


				in.seekg(startpos); in.seekg(hdr.strlist_offs, std::ios::cur);
	            //djutil::containers::vectorlist<std::basic_string<char32_t>> strlist = {};
				this->stringlist.resize(hdr.strlist_numents);
				for (auto& decstr : this->stringlist) {
					const size_t stridx = ((&decstr) - (&this->stringlist.front()));
					uint32_t fstrsz, declen;
					_unpackLE(&fstrsz, 1);
					_unpackLE(&declen, 1);
					std::string encstr; encstr.resize(fstrsz);
					if (in.read(&encstr.front(), fstrsz).gcount() != fstrsz){
						throw std::runtime_error("premature EOF encountered in string list.");
					}

					switch (hdr.strlist_encoding) {
						case 1: {
							decstr.resize(declen);
							const auto* uc = (unsigned char*)(encstr.data());
							for (size_t i = 0; i < declen; i++) {decstr[i] = char32_t(uc[i]);}
							break;
						}
						case 2: decstr = ezstr::utf8_to_utf32(encstr); break;
						default: decstr = ezstr::ascii_to_utf32(encstr);
					}

				}

				in.seekg(startpos); in.seekg(hdr.fcb_offs, std::ios::cur);
				auto fcb_start = in.tellg();
				in.seekg(startpos); in.seekg(hdr.ena_offs, std::ios::cur);
				auto ena_start = in.tellg();

				in.seekg(startpos); in.seekg(hdr.nodedata_offs, std::ios::cur);
				auto nodes_start = in.tellg();

				this->fsnodes.resize(hdr.total_num_nodes, nullptr);
				this->nodedatas.resize(hdr.total_num_nodes, _rpksrc_nodedata{});
				std::streampos nextnode_pos = nodes_start, nextena_pos = ena_start;
				struct {
					uint32_t nodeindex, num_chunks;
					rpkpos_t datalen;
				} ena_ent_hdr;
				for (_rpksrc_nodedata& cnd : this->nodedatas) {
					//in.seekg(nextnode_pos);
					unsigned char _nodetype = 0;
					binio::unpack(&_nodetype, in, 1, true);
					cnd.nodetype = _rpknodetype(_nodetype);
					//in.seekg(3, std::ios::cur);
					binio::unpack(cnd._pad1, in, 3);

					_unpackLE(&cnd.nodeindex, 1);
					_unpackLE(&cnd.parentnodeindex, 1);
					_unpackLE(&cnd.fullnameindex, 1);
					_unpackLE(&cnd.tot_num_childnodes, 1);
					_unpackLE(&cnd.num_subdirs, 1);
					_unpackLE(&cnd.num_files, 1);

					_read_pos_t(&cnd.offset, 1);
					_read_pos_t(&cnd.size, 1);
					if (cnd.nodetype == _rpknodetype::file) {
					    //cnd.offset += size_t(fcb_start);
					    //cnd.offset += rpkpos_t(startpos);
					    cnd.offset += rpkpos_t(startpos);
					}
					std::cout << ezstr::utf32_to_utf8(this->stringlist[cnd.fullnameindex]) << " -> offset = " << cnd.offset << ", size = " << cnd.size << "\n";
					/*
					cnd.compression = _rpkcomprtype::none;
					cnd.uncompressed_size = cnd.size;

					nextnode_pos = in.tellg();
					in.seekg(nextena_pos);

					_unpackLE(&ena_ent_hdr.nodeindex, 2);
					_read_pos_t(&ena_ent_hdr.datalen, 1);
					std::string chunktype = ""; chunktype.resize(8);
					//std::string chunkdata = "";
					uint32_t chunkdatlen = 0;
					for (uint32_t chunkno = 0; chunkno < ena_ent_hdr.num_chunks; chunkno++) {
					    binio::unpack(&chunktype.front(), in, chunktype.size());
						_unpackLE(&chunkdatlen, 1);

						if (chunktype == "COMPTYPE") {
							binio::unpack<_rpkcomprtype, uint32_t>(
							    &cnd.compression,
							    in,
							    1,
							    true
							);
						}
						else if (chunktype == "UNCMPRSZ") {
							_read_pos_t(&cnd.uncompressed_size, 1);
						}
						else {
							in.seekg(chunkdatlen, std::ios::cur);
						}
					}

					nextena_pos = in.tellg();
					*/
					this->init_node(cnd.nodeindex);
				}
				this->_entree_fsnodes();


				#undef _unpackLE
				#undef _read_pos_t
			}
		public:
		    virtual src_type getSourceType(){return src_type::rpksrc;}
		    virtual void _on_deletion() {
				if (this->arc_istr == (&this->_arc_ifstr)) {
					this->_arc_ifstr.close();
				}
				else {
				    delete[] this->arc_istr;
				}
				this->arc_istr = nullptr;

				delete this->root;
				this->root = nullptr;

				this->fsnodes.clear();
				this->nodedatas.clear();
				this->dirnodes.clear();
				this->filenodes.clear();

			}

			virtual ent_type getEntityType(const fspath_t& ent) {
				if (this->dirnodes.count(ent) > 0){return ent_type::dir;}
				else if (this->filenodes.count(ent) > 0){return ent_type::file;}
				else {return ent_type::nonexistent;}
			}

			virtual bool getFileMemberReadHandle(vfs_isubstreambuf& sb, const fspath_t& ent) {
				if (this->filenodes.count(ent) == 0){return false;}
				_rpksrc_fsnode* fnode = this->filenodes[ent];
				sb = vfs_isubstreambuf(this->arc_istr->rdbuf(), fnode->data->offset, fnode->data->size);
				return true;
			}

			virtual bool copyFileMemberToSStream(std::stringstream& sstr, const fspath_t& ent) {
				if (this->filenodes.count(ent) == 0){return false;}
				_rpksrc_fsnode* fnode = this->filenodes[ent];
				this->arc_istr->seekg(fnode->data->offset);
				std::string _cpbuf; _cpbuf.resize(fnode->data->size);
				this->arc_istr->read(&_cpbuf.front(), _cpbuf.size());
				sstr.write(&_cpbuf.front(), _cpbuf.size());
				return true;
			}

			virtual bool getDirectoryListing(fsdirlist_t& out_dirs, fsdirlist_t& out_files, const fspath_t& ent) {
				if (this->dirnodes.count(ent) == 0){return false;}
				_rpksrc_fsnode* dnode = this->dirnodes[ent];

				for (_rpksrc_fsnode* const& child : dnode->children) {
					fspathpair_t pp = {};
				    pp.relname = child->data->name;
				    pp.fullname = child->data->fullname;
				    fsdirlist_t* lst = nullptr;
					switch (child->data->nodetype) {
						case _rpknodetype::file: lst = &out_files; break;
					    default: lst = &out_dirs; break;
					}
					lst->push_back(pp);
				}
				return true;
			}

			ResourcePackSource(VirtualFS* fs, const fspath_t& arcpath) : VirtualFSSourceBase(fs) {
				if (!this->_arc_ifstr.open(arcpath, std::ios::in | std::ios::binary)) {
					std::stringstream errss;
					errss << "can't open file for reading: " << ezstr::utf32_to_utf8(arcpath);
					throw std::runtime_error(errss.str());
				}
				this->arc_istr = &this->_arc_ifstr;
				this->_read_archive();
			}
	};

	/*
	struct rpksrc_nodedata {
		std::u32string name = U"", fullname = U"";
		bool isdir = false;
		size_t ofs = 0, size = 0;
	};
	*/
	struct _pk2src_nodedata {
		fspath_t name = U"", fullname = U"";
		bool isdirnode = false;
		size_t fmofs = 0, fmsize = 0;
	};
	using _pk2source_fsnode = containers::treenode<_pk2src_nodedata>;
	/*
	class _pk2source_fsnode : public containers::treenode<_pk2src_nodedata> {
		using containers::treenode<_pk2src_nodedata>::treenode;
		public:
		    static _pk2source_fsnode* new_dirnode(const fspath_t& _name, _pk2source_fsnode* _parentdir=nullptr) {
				auto* nn = new _pk2source_fsnode();
				nn->data = _pk2src_nodedata{};
				nn->data.name = _name;
				nn->data.isdirnode = true;
				if (_parentdir != nullptr) {_parentdir->addchild(nn);}
				return nn;
			}

			static _pk2source_fsnode* new_filenode(const fspath_t& _name, _pk2source_fsnode* _parentdir, const size_t _fmofs, const size_t _fmsize) {
				if (_parentdir == nullptr || !_parentdir->data.isdirnode){throw std::runtime_error("tried to create PK2 filenode without a valid parent dirnode!");}
				auto* nn = new _pk2source_fsnode();
				nn->data.is
			}
	};
	*/
	struct _pk2src_findbyname {
		bool matches_dirnodes, matches_filenodes;
		fspath_t name2match;

		inline bool operator()(_pk2source_fsnode* n) const {
			if (n == nullptr){return false;}
			else if (n->data.isdirnode && !this->matches_dirnodes){return false;}
			else if (!n->data.isdirnode && !this->matches_filenodes){return false;}
			return n->data.name == this->name2match;
		}
		_pk2src_findbyname(const fspath_t name=U"", const bool _matches_dirnodes=true, const bool _matches_filenodes=true) :
		    name2match(name),
		    matches_dirnodes(_matches_dirnodes),
		    matches_filenodes(_matches_filenodes)
		{}

	};
	class PK2Source : public VirtualFSSourceBase {
		using VirtualFSSourceBase::VirtualFSSourceBase;
		protected:
		    _pk2source_fsnode* root_dirnode = nullptr;
		    //std::ifstream _arc_ifstr;
		    djutil::ufpio::ufstream _arc_ifstr;
		    std::istream* arcfp = nullptr;
		    bool autoclose = false;
		    int _archive_version_number = 0;
		    containers::dictionary<fspath_t, _pk2source_fsnode*> _dirnodes_bypath = {};
		    containers::dictionary<fspath_t, _pk2source_fsnode*> _filenodes_bypath = {};
		    void _set_fullname(_pk2source_fsnode* n) {
				if (n->parents.size() == 0){n->data.name = U""; n->data.fullname = U"/";}
				else {
					_pk2source_fsnode* p = n->parents.front();
					n->data.fullname = p->data.fullname + n->data.name;
					if (n->data.isdirnode && (n->data.fullname.back() != U'/')){n->data.fullname += U"/";}
				}
				if (n->data.isdirnode){this->_dirnodes_bypath[n->data.fullname] = n;}
				else {this->_filenodes_bypath[n->data.fullname] = n;}
			}
			_pk2source_fsnode* _ensure_dirnode(const fspath_t& dirname) {
				if (this->_dirnodes_bypath.count(dirname) > 0){return this->_dirnodes_bypath[dirname];}
				_pk2source_fsnode* cur = this->root_dirnode;
				auto comps = ezstr::split<char32_t>(dirname, U"/");

				for (auto& subn : comps) {
					if (subn == U""){continue;}
					_pk2source_fsnode* first = nullptr;
					_pk2source_fsnode::nodelist_type found = {};
					cur->find_childnodes(found, _pk2src_findbyname(subn, true, false), 1);
					if (found.size() > 0) {
						first = found.front();
					}

				    if (first == nullptr) {
						first = new _pk2source_fsnode();
						first->data = _pk2src_nodedata{
							.name=subn, .isdirnode=true
						};
						cur->addchild(first);
						this->_set_fullname(first);
					}
					cur = first;
				}
				return cur;
			}
			void _read_archive() {
				std::istream& istr = *this->arcfp;
				std::streampos startpos = istr.tellg();
				char ident[4] = {};
				binio::unpack(ident, istr, 4);
				if (!memcmp(ident, "PK22", 4)){this->_archive_version_number = 22;}
				else if (!memcmp(ident, "PK23", 4)){this->_archive_version_number = 23;}
				else {throw std::runtime_error("Not a PK22 or PK23 archive!");}

				//std::cout << "info: PK2 archive version: " << this->_archive_version_number << '\n';

				containers::vectorlist<fspath_t> strlist = {};
				int cstri = 0;
				switch (this->_archive_version_number) {
					case 22: {
						istr.seekg(8, std::ios::cur);
						while (istr.get() != 0) {
							//std::cout << "info: read PK22 string " << cstri << '\n';
							uint32_t nb = 0;
							binio::unpack(&nb, istr, 1, true);
							std::string _rd; _rd.resize(nb);
							binio::unpack(&(_rd.front()), istr, nb);
							strlist.push_back(ezstr::utf8_to_utf32(_rd));
							//ezstr::mb2wide(strlist.back(), _rd);
							cstri++;
						}
						break;
					}
					case 23: {
						uint32_t nstrs;
						uint8_t encoding; // 0 = ascii, 1 = utf-8
						binio::unpack(&nstrs, istr, 1, true);
						binio::unpack(&encoding, istr, 1);
						strlist.resize(nstrs);
						for (auto& cstr : strlist) {
							//std::cout << "info: read PK23 string " << cstri << '\n';
							uint32_t declen, nbytes;
							binio::unpack(&declen, istr, 1, true);
							binio::unpack(&nbytes, istr, 1, true);

							std::string _rd; _rd.resize(nbytes);
							strlist.push_back(U"");
							binio::unpack(&(_rd.front()), istr, nbytes);

							if (encoding == 0){
								strlist.back() = ezstr::ascii_to_utf32(_rd);
							}
							else if (encoding == 1){
								strlist.back() = ezstr::utf8_to_utf32(_rd);
							}

							else {throw std::runtime_error("unknown encoding: " + std::to_string(uint32_t(encoding)));}
							cstri++;
						}

						break;
					}
					default: ;
				}
				//std::cout << "info: creating root directory node.\n";
				this->root_dirnode = new _pk2source_fsnode();
				this->root_dirnode->data = _pk2src_nodedata{
					.isdirnode=true
				};
				this->_set_fullname(this->root_dirnode);
				//std::cout << "info: created root dir node!\n";

				size_t numdirs;
				binio::unpack<size_t, uint64_t>(&numdirs, istr, 1, true);
				//std::cout << "info: numdirs = " << numdirs << '\n';
				_pk2source_fsnode::nodelist_type dirnodelist;
				dirnodelist.resize(numdirs + 1);
				dirnodelist[0] = this->root_dirnode;
				for (size_t i = 0; i < numdirs; i++) {
					size_t nodeindex = i+1;
					//std::cout << "info: reading dirnode " << i << " (nodeindex = " << nodeindex << ")\n";
					_pk2source_fsnode* ndn = nullptr;
					if (this->_archive_version_number == 22) {
						size_t nameindex, parentnameindex;
						binio::unpack<size_t, uint64_t>(&nameindex, istr, 1, true);
						binio::unpack<size_t, uint64_t>(&parentnameindex, istr, 1, true);
						fspath_t fullname = strlist[parentnameindex] + strlist[nameindex] + U"/";
						ndn = this->_ensure_dirnode(fullname);
					}
					else if (this->_archive_version_number == 23) {
						size_t nameindex, parentnodeindex;
						binio::unpack<size_t, uint64_t>(&nameindex, istr, 1, true);
						binio::unpack<size_t, uint64_t>(&parentnodeindex, istr, 1, true);
						ndn = new _pk2source_fsnode();
						ndn->data = _pk2src_nodedata{
							.name=strlist[nameindex],
							.isdirnode=true
						};
						auto* pdn = dirnodelist[parentnodeindex];
						pdn->addchild(ndn);
						this->_set_fullname(ndn);
					}
					dirnodelist[nodeindex] = ndn;
				}
				size_t numfiles;
				binio::unpack<size_t, uint64_t>(&numfiles, istr, 1, true);

				std::streampos fcb_startpos = istr.tellg();
				fcb_startpos += std::streampos(numfiles * 32);
				//std::cout << "info: numfiles = " << numfiles << '\n';
				for (size_t i = 0; i < numfiles; i++) {
					//std::cout << "info: read filenode " << i << '\n';
					_pk2source_fsnode *pdn = nullptr, *nfn = nullptr;
					size_t nums[4] = {};
					binio::unpack<size_t, uint64_t>(nums, istr, 4, true);
					nfn = new _pk2source_fsnode();
					const size_t absoffs = size_t(fcb_startpos) + nums[2];
					nfn->data = _pk2src_nodedata{
						.name=strlist[nums[0]],
						.isdirnode=false,
						.fmofs=absoffs,
						.fmsize=nums[3]
					};
					/*
					std::cout << "info: filenode->data.name = U" << std::quoted(ezstr::utf32_to_utf8(nfn->data.name)) << '\n';
					std::cout << "info: filenode->data.fmofs = " << size_t(nfn->data.fmofs) << '\n';
					std::cout << "info: filenode->data.fmsize = " << nfn->data.fmsize << '\n';
					*/
					if (this->_archive_version_number == 22) {
						//std::cout << "info: filenode's parent name = U" << std::quoted(ezstr::utf32_to_utf8(strlist[nums[1]])) << '\n';
						pdn = this->_dirnodes_bypath[strlist[nums[1]]];
						pdn->addchild(nfn);
						this->_set_fullname(nfn);
					}
					else if (this->_archive_version_number == 23){
						//std::cout << "info: filenode's parent index = " << nums[1] << '\n';
						pdn = dirnodelist[nums[1]];
						pdn->addchild(nfn);
						this->_set_fullname(nfn);
					}
				}
			}
		public:
		    virtual src_type getSourceType(){return src_type::pk2src;}
		    virtual void _on_deletion() {
				if ((this->_arc_ifstr) && this->autoclose){
					this->_arc_ifstr.close();
				}
				this->arcfp = nullptr;
				this->_dirnodes_bypath.clear();
				this->_filenodes_bypath.clear();
				delete this->root_dirnode;
				this->root_dirnode = nullptr;
				this->_archive_version_number = 0;
			}
			virtual ent_type getEntityType(const fspath_t& fsp) {
				if (this->_filenodes_bypath.count(fsp) > 0){return ent_type::file;}
				else if (this->_dirnodes_bypath.count(fsp) > 0){return ent_type::dir;}
				else {return ent_type::nonexistent;}
			}
			virtual bool getFileMemberReadHandle(vfs_isubstreambuf& istr, const fspath_t& entname) {
				if (this->_filenodes_bypath.count(entname) == 0){return false;}
				auto fnp = this->_filenodes_bypath[entname];
				istr = vfs_isubstreambuf(this->arcfp->rdbuf(), fnp->data.fmofs, fnp->data.fmsize);
				return true;
			}
			virtual bool getDirectoryListing(fsdirlist_t& out_dirs, fsdirlist_t& out_files, const fspath_t& fsp) {
				if (this->getEntityType(fsp) != ent_type::dir){return false;}
				auto dn = (this->_dirnodes_bypath[fsp]);
				for (auto n : dn->children) {
					fspathpair_t pp = {
						.fullname=n->data.fullname,
						.relname=n->data.name
					};
					fsdirlist_t& l2p = (n->data.isdirnode ? out_dirs : out_files);
					l2p.push_back(pp);
				}
				return true;
			}
			PK2Source(VirtualFS* _vfsi, const fspath_t& fp) : VirtualFSSourceBase(_vfsi), autoclose(true) {
				this->_arc_ifstr.open(fp, std::ios::in | std::ios::binary);
				if (!this->_arc_ifstr){
					throw std::runtime_error("can't open file for reading: " + ezstr::utf32_to_utf8(fp));
				}
				this->arcfp = &this->_arc_ifstr;
				this->_read_archive();
			}
			PK2Source(VirtualFS* _vfsi, std::istream& istr) : VirtualFSSourceBase(_vfsi), autoclose(false) {
				this->arcfp = &istr;
				this->_read_archive();
			}

	};
	struct _q1paksrc_fsnode {
		std::u32string name = U"", fullname = U"";
		std::string pakname = "";
		bool isdir = true;
		_q1paksrc_fsnode* parent = nullptr;
		containers::vectorlist<_q1paksrc_fsnode*> children = {};
		int32_t ofs = 0, size = 0;
		_q1paksrc_fsnode* operator[](const fspath_t& n) {
			for (auto*& c : this->children) {
				if (c->name == n){return c;}
			}
			return nullptr;
		}
		~_q1paksrc_fsnode() {
			while (this->children.size() > 0){
				delete this->children.back();
				this->children.pop_back();
			}
		}
	};

	class QuakePAKSource : public VirtualFSSourceBase {
		using VirtualFSSourceBase::VirtualFSSourceBase;
		protected:
		    _q1paksrc_fsnode* rootdir = nullptr;
		    std::ifstream _arcfile;
		    std::ifstream* archandle = nullptr;
		    std::unordered_map<fspath_t, _q1paksrc_fsnode*> nodemap = {};
		    _q1paksrc_fsnode* find_node(const fspath_t& ent)
		    {
		    	if (this->rootdir == nullptr){return nullptr;}
		    	else if (ent == U"/"){return this->rootdir;}
		    	return this->nodemap[ent];
		    }
		    void _fspath2pakname(std::string& out_pakname, const fspath_t& in_fspath)
		    {
		    	fspath_t fsp = in_fspath;
		    	std::string pakn = "";
		    	if (fsp.front() == U'/'){fsp.erase(fsp.begin());}
		    	ezstr::wide2mb<char, char32_t>(pakn, fsp);
		    	out_pakname = pakn;
		    }
		    void _pakname2fspath(fspath_t& out_fspath, const std::string& in_pakname) {
		    	std::string pakn = "/" + in_pakname;
		    	fspath_t fsp = U"";
		    	//ezstr::mb2wide<char32_t, char>(fsp, pakn);
		    	//for (const auto& c : pakn){fsp.push_back((char32_t)(unsigned char)(c));}
		    	fsp.resize(pakn.size());
		    	std::copy_n((unsigned char*)(pakn.data()), pakn.size(), &fsp[0]);
		    	out_fspath = fsp;
		    }
		    _q1paksrc_fsnode* ensure_dir(const fspath_t& ent) {
		    	if (ent == U"/" || this->rootdir == nullptr){return this->rootdir;}
		    	std::vector<fspath_t> tokens = ezstr::split<char32_t>(ent, U"/");
		    	_q1paksrc_fsnode* curnode = this->rootdir;
		    	for (auto& tok : tokens) {
		    	    if (tok == U""){continue;}
		    		bool gotit = false;
		    		for (auto*& cand : curnode->children) {
		    			if (cand->isdir && cand->name == tok) {curnode = cand; gotit = true; break;}
		    		}
		    		if (!gotit) {
		    			_q1paksrc_fsnode* ndir = new _q1paksrc_fsnode();
		                ndir->parent = curnode;
		                ndir->pakname = "";
		                ndir->name = tok;
		                ndir->fullname = (curnode->fullname)+(tok)+(U"/");
		                this->nodemap[ndir->fullname] = ndir;
		                ndir->isdir = true;
		                this->_fspath2pakname(ndir->pakname, ndir->fullname);
		                std::string _pfp; ezstr::wide2mb(_pfp, ndir->fullname);
		                std::cout << "new dirnode: " << _pfp << '\n';
		                ndir->children.clear();
		                curnode->children.push_back(ndir);
		                curnode = ndir;

		                continue;
		    		}
		    		else {
		    			continue;
		    		}

		    	}
		    	return curnode;
		    }
		    void read_archive() {
		        this->rootdir = new _q1paksrc_fsnode();
		        this->rootdir->name = U"";
		        this->rootdir->fullname = U"/";
		        this->rootdir->parent = nullptr;
		        this->rootdir->isdir = true;
		        this->rootdir->pakname = "";
		        this->nodemap[U"/"] = this->rootdir;

		    	auto& arc = *(this->archandle);
		    	const char _pack_ident[4] = {'P', 'A', 'C', 'K'};
		    	char id_rd[4] = {};
		        if (arc.read(id_rd, 4).gcount() < 4 || memcmp(id_rd, _pack_ident, 4) != 0){throw std::runtime_error("Not a PAK file!");}
		        int32_t ftoffs, ftsize;
		        binio::unpack(&ftoffs, arc, 1, true);
		        binio::unpack(&ftsize, arc, 1, true);

		        struct {
		        	char name[56];
		        	int32_t ofs, size;
		        } fent;

		        arc.seekg(ftoffs);

		        for (int i = 0; i < (ftsize/64); i++)
		        {
		        	binio::unpack(fent.name, arc, 56);
		        	binio::unpack(&fent.ofs, arc, 1, true);
		        	binio::unpack(&fent.size, arc, 1, true);
		        	std::string pakname(fent.name, (fent.name[55] != 0 ? 56 : strlen(fent.name)));
		        	//std::cout << std::quoted(pakname) << ", ofs=" << fent.ofs << ", size=" << fent.size << '\n';

		        	fspath_t fspath = U"", fsdir = U"", fsfn = U"";
		        	this->_pakname2fspath(fspath, pakname);

		        	ezstr::rpartition<char32_t>(fsdir, fsfn, fspath, U"/");
		        	fsdir += U"/";
		        	//std::string _fspath_p; ezstr::wide2mb(_fspath_p, fspath); std::cout << "fspath=" << std::quoted(_fspath_p) << '\n';
		            auto* dirnode = this->ensure_dir(fsdir);
		            _q1paksrc_fsnode* nnode = new _q1paksrc_fsnode();
		            nnode->parent = dirnode;
		            nnode->pakname = pakname;
		            nnode->fullname = fspath;
		            nnode->name = fsfn;
		            nnode->isdir = false;
		            nnode->ofs = fent.ofs;
		            nnode->size = fent.size;
		            dirnode->children.push_back(nnode);
		            this->nodemap[nnode->fullname] = nnode;
		        }
		    }
		public:
		    virtual src_type getSourceType(){return src_type::qpaksrc;}
		    virtual void _on_deletion() {
		    	this->_arcfile.close();
		    	delete this->rootdir; this->rootdir = nullptr;
		    }
		    virtual bool getDirectoryListing(fsdirlist_t& out_dirs, fsdirlist_t& out_files, const fspath_t& entpath)
		    {
		    	auto* dirnode = this->find_node(entpath);
		    	if (dirnode == nullptr || !dirnode->isdir){return false;}
		    	for (auto*& c : dirnode->children)
		    	{
		    		if (c->isdir) {out_dirs.push_back(fspathpair_t{.fullname=c->fullname, .relname=c->name});}
		    		else {out_files.push_back(fspathpair_t{.fullname=c->fullname, .relname=c->name});}
		    	}
		    	return true;
		    }
		    virtual bool getFileMemberReadHandle(vfs_isubstreambuf& istr, const fspath_t& entname) {
		    	auto* fnode = this->find_node(entname);
		    	if (fnode == nullptr || fnode->isdir) {
		    		return false;
		    	}
		    	istr = vfs_isubstreambuf(this->_arcfile.rdbuf(), fnode->ofs, fnode->size);
		    	return true;
		    }
		    QuakePAKSource(VirtualFS* _vfsi, const fspath_t pak) : VirtualFSSourceBase(_vfsi) {
		    	std::string pakfn = "";
		    	ezstr::wide2mb<char, char32_t>(pakfn, pak);
		    	this->archandle = &this->_arcfile;
		    	this->_arcfile.open(pakfn, std::ios::binary);
		    	if (!this->_arcfile) {
		    		throw std::runtime_error("can't open file: " + pakfn);
		    	}

		    	this->read_archive();
		    }
	};
	class vfistream : public std::istream {
		private:
		    vfs_isubstreambuf* _istr = nullptr;
		    VirtualFS* _cvfs = nullptr;
		public:

		    void close() {
		        //this->clear();
		    	this->rdbuf(nullptr);
		    	delete this->_istr; this->_istr = nullptr;
		    	this->_cvfs = nullptr;
		    }
		    vfistream& open(VirtualFS& vfs, const fspath_t& p, const std::ios::openmode om=std::ios::binary) {
		    	this->close();

		    	this->_cvfs = &vfs;
		    	this->_istr = new vfs_isubstreambuf;
		    	vfs.open(*(this->_istr), p);

		    	this->rdbuf(this->_istr);
		    	if (om & std::ios::ate) {this->seekg(0, std::ios::end);}
		    	return *this;
		    }
		    vfistream() : std::istream(nullptr) {}
		    vfistream(VirtualFS& vfs, const fspath_t& p, const std::ios::openmode om=std::ios::binary) : std::istream(nullptr) {
		    	this->open(vfs, p, om);
		    }
		    ~vfistream() {
		    	this->close();
		    }

	};
}};
#endif
