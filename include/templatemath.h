/*
 * templatemath.h
 *
 *  Created on: Jan 1, 2016
 *      Author: agibsonccc
 */

#ifndef TEMPLATEMATH_H_
#define TEMPLATEMATH_H_

#include <math.h>
#include <dll.h>

#ifdef __CUDACC__
#include <types/float16.h>
#define math_def __host__ __device__

typedef union {
		struct {
			half H;
			half L;
		} B;
		int W;
} PAIR;
#else
#define math_def
#include <types/float16.h>
#endif

namespace nd4j {
#ifdef __CUDACC__

#endif

	namespace math {
		template<typename T>
		math_def inline T nd4j_abs(T value);

template<typename T>
        math_def inline void nd4j_swap(T &val1, T &val2);

		template<typename T>
        math_def inline T nd4j_max(T val1, T val2);

		template<typename T>
        math_def inline T nd4j_min(T val1, T val2);

#ifndef __CUDACC__
        template<typename T>
        math_def inline T nd4j_dot(T *x, T *y, int length);
#endif

		template<typename T>
        math_def inline T nd4j_ceil(T val1);

		template<typename T>
        math_def inline T nd4j_cos(T val);

        template<typename T>
        math_def inline T nd4j_cosh(T val);

		template<typename T>
        math_def inline T nd4j_exp(T val);

		template<typename T>
        math_def inline T nd4j_floor(T val);

		template<typename T>
        math_def inline T nd4j_log(T val);

		template<typename T>
        math_def inline T nd4j_pow(T val, T val2);

		template<typename T>
        math_def inline T nd4j_round(T val);

        template<typename T>
        math_def inline T nd4j_remainder(T num, T denom);

        template<typename T>
        math_def inline T nd4j_fmod(T num, T denom);

		template<typename T>
        math_def inline T nd4j_sigmoid(T val) {
			return (T) 1.0 / ((T) 1.0 + nd4j_exp<T>(-val));
		}

		template<typename T>
        math_def inline T nd4j_elu(T val) {
			if (val >= (T) 0.0) return val;
			else return nd4j_exp<T>(val) - (T) 1.0;
			//return val >= 0.0 ? val : (nd4j_exp<T>(val) - 1.0);
		}


		template<typename T>
        math_def inline T nd4j_leakyrelu(T val,T alpha) {
			if (val < (T) 0.0f) return alpha * val;
			else return val;
			//return val < 0 ?  alpha * val : val;
		}


		template<typename T>
        math_def inline T nd4j_eluderivative(T val) {
			if (val >= (T) 0.0f) return (T) 1.0f;
			else return nd4j_exp<T>(val);
			//return val >= 0.0 ? 1.0 : nd4j_exp(val);
		}
		template<typename T>
        math_def inline T nd4j_sin(T val);

		template<typename T>
		math_def inline T nd4j_sinh(T val);
		template<typename T>
        math_def inline T softplus(T val) {
			return nd4j_log<T>((T) 1.0f + nd4j_exp<T>(val));
		}
		template<typename T>
        math_def inline T nd4j_softsign(T val) {
			return val / ((T) 1.0f + nd4j::math::nd4j_abs<T>(val));
		}

		template<typename T>
        math_def inline T nd4j_sqrt(T val);

		template<typename T>
        math_def inline T nd4j_tanh(T val);
        template<typename T>
        math_def inline T nd4j_tan(T val);

        template<typename T>
        math_def inline T nd4j_tan(T val) {
            return nd4j_log((val + 1 / (1 - val)) * 0.5);
        }

        template<typename T>
        math_def inline T nd4j_tanhderivative(T val) {
			T tanh = nd4j_tanh(val);
			return (T) 1.0f - tanh * tanh;
		}
		template<typename T>
        math_def inline T nd4j_sigmoidderivative(T val) {
			T sigmoid = nd4j_sigmoid(val);
			T out = sigmoid * ((T) 1.0f - sigmoid);
			return out;
		}

		template<typename T>
        math_def inline T nd4j_softsignderivative(T val) {
			T y = (T) 1.0f + nd4j_abs(val);
			return (T) 1.0f / (y * y);
		}

        template<typename T>
        math_def inline T nd4j_sgn(T val) {
            return val < (T) 0.0f ? (T) -1.0f : val > (T) 0.0f ? (T) 1.0f : (T) 0.0f;
        }

        template<typename T>
        math_def inline T nd4j_sign(T val) {
            return nd4j_sgn<T>(val);
        }

        template<typename T>
        math_def inline T nd4j_signum(T val) {
            return nd4j_sgn<T>(val);
        }

#ifndef __CUDACC__

        template<>
        math_def inline float16 nd4j_dot<float16>(float16 *x, float16 *y, int length) {
            float16 dot = (float16) 0.0f;

            // TODO: since we can't use simd on unions, we might use something else here.
            for(int e = 0; e < length; e++) {
                dot += x[e] * y[e];
            }

            return dot;
        }

		template<typename T>
        math_def inline T nd4j_dot(T *x, T *y, int length) {
            T dot = (T) 0.0f;

#pragma omp simd reduction(+:dot)
			for(int e = 0; e < length; e++) {
				dot += x[e] * y[e];
			}

			return dot;
		}
#endif

		template<typename T>
        math_def inline T nd4j_acos(T val);

		template<typename T>
        math_def inline T nd4j_asin(T val);

        template<typename T>
        math_def inline T nd4j_asinh(T val) {
            //Math.log(Math.sqrt(Math.pow(x, 2) + 1) + x)
            return nd4j_log(nd4j_sqrt(nd4j_pow(val,2) + 1) + val);
        }

		template<typename T>
        math_def inline T nd4j_atan(T val);

        template<typename T>
        math_def inline T nd4j_atanh(T val);


        template<>
        math_def inline float16 nd4j_abs<float16>(float16 value) {
#ifdef NATIVE_HALFS
            return value < 0. ?  __hneg(value.data) : value;
#else
			return (float16) fabsf((float) value);
#endif
		}


		template<>
        math_def inline float nd4j_abs<float>(float value) {
			return fabsf(value);
		}

		template<>
        math_def inline double nd4j_abs<double>(double value) {
			return value < 0 ? -value : value;
		}

		template<>
        math_def inline int nd4j_abs<int>(int value) {
			return value < 0 ? -value : value;
		}


		template<>
        math_def inline float16 nd4j_max<float16>(float16 val1, float16 val2) {
			return val1 > val2 ? val1 : val2;
		}


		template<>
        math_def inline float nd4j_max<float>(float val1, float val2) {
			return val1 > val2 ? val1 : val2;
		}

		template<>
        math_def inline double nd4j_max<double>(double val1, double val2) {
			return val1 > val2 ? val1 : val2;
		}

		template<>
        math_def inline int nd4j_max<int>(int val1, int val2) {
			return val1 > val2 ? val1 : val2;
		}


		template<>
        math_def inline float16 nd4j_min<float16>(float16 val1, float16 val2) {
			return val1 < val2 ? val1 : val2;
		}


		template<>
        math_def inline float nd4j_min<float>(float val1, float val2) {
			return val1 < val2 ? val1 : val2;
		}

		template<>
        math_def inline double nd4j_min<double>(double val1, double val2) {
			return val1 < val2 ? val1 : val2;
		}
		template<>
        math_def inline int nd4j_min<int>(int val1, int val2) {
			return val1 < val2 ? val1 : val2;
		}

		template<>
        math_def inline float16 nd4j_ceil<float16>(float16 val) {
#ifdef NATIVE_HALFS
            return hceil(val.data)
#else
			return ceilf((float) val);
#endif
		}

		template<>
        math_def inline float nd4j_ceil<float>(float val1) {
			return ceilf(val1);
		}

		template<>
        math_def inline double nd4j_ceil<double>(double val) {
			return ceil(val);
		}

		template<>
        math_def inline int nd4j_ceil<int>(int val) {
			return ceil((float) val);
		}

		template<>
        math_def inline float16 nd4j_cos<float16>(float16 val) {
#ifdef NATIVE_HALFS
			return hcos(val.data);
#else
			return cosf((float) val);
#endif
		}

		template<>
        math_def inline float nd4j_cos<float>(float val) {
			return cosf(val);
		}

		template<>
        math_def inline double nd4j_cos<double>(double val) {
			return cos(val);
		}

		template<>
        math_def inline int nd4j_cos<int>(int val) {
			return cosf((float) val);
		}


        template<>
        math_def inline float16 nd4j_cosh<float16>(float16 val) {
            return coshf((float) val);
        }

        template<>
        math_def inline float nd4j_cosh<float>(float val) {
            return coshf(val);
        }

        template<>
        math_def inline double nd4j_cosh<double>(double val) {
            return cosh(val);
        }

        template<>
        math_def inline int nd4j_cosh<int>(int val) {
            return coshf((float) val);
        }


		template<>
        math_def inline float16 nd4j_exp<float16>(float16 val) {
#ifdef NATIVE_HALFS
            return hexp(val.data);
#else
			return (float16) expf((float) val);
#endif
		}


		template<>
        math_def inline float nd4j_exp<float>(float val) {
			return expf(val);
		}

		template<>
        math_def inline double nd4j_exp<double>(double val) {
			return exp(val);
		}

		template<>
        math_def inline int nd4j_exp<int>(int val) {
			return expf((float) val);
		}

		template<>
        math_def inline float16 nd4j_floor<float16>(float16 val) {
#ifdef NATIVE_HALFS
            return hfloor(val.data);
#else
			return (float16) floorf((float) val);
#endif
		}


		template<>
        math_def inline float nd4j_floor<float>(float val) {
			return floorf(val);
		}

		template<>
        math_def inline double nd4j_floor<double>(double val) {
			return floor(val);
		}

		template<>
        math_def inline int nd4j_floor<int>(int val) {
			return floorf((float) val);
		}


		template<>
        math_def inline float16 nd4j_log<float16>(float16 val) {
#ifdef NATIVE_HALFS
            return hlog(val.data);
#else
			return (float16) logf((float) val);
#endif
		}


		template<>
        math_def inline float nd4j_log<float>(float val) {
			return logf(val);
		}

		template<>
        math_def inline double nd4j_log<double>(double val) {
			return log(val);
		}

		template<>
        math_def inline int nd4j_log<int>(int val) {
			return logf((int) val);
		}


		template<>
        math_def inline float16 nd4j_pow<float16>(float16 val, float16 val2) {
			return (float16) powf((float) val, (float) val2);
		}

		template<>
        math_def inline float nd4j_pow<float>(float val, float val2) {
			return powf(val, val2);
		}

		template<>
        math_def inline double nd4j_pow<double>(double val, double val2) {
			return pow(val, val2);
		}

		template<>
        math_def inline int nd4j_pow<int>(int val, int val2) {
			return powf((float) val, (float) val2);
		}

		template<>
        math_def inline float16 nd4j_round<float16>(float16 val) {
			return (float16) roundf((float) val);
		}


		template<>
        math_def inline float nd4j_round<float>(float val) {
			return roundf(val);
		}

        template<>
        math_def inline float nd4j_remainder<float>(float num, float denom) {
            return remainderf(num, denom);
        }

        template<>
        math_def inline double nd4j_remainder<double>(double num, double denom) {
            return remainder(num, denom);
        }

        template<>
        math_def inline float16 nd4j_remainder<float16>(float16 num, float16 denom) {
            return (float16) remainderf((float) num, (float) denom);
        }


        template<>
        math_def inline float nd4j_fmod<float>(float num, float denom) {
            return fmodf(num, denom);
        }

        template<>
        math_def inline double nd4j_fmod<double>(double num, double denom) {
            return fmod(num, denom);
        }

        template<>
        math_def inline float16 nd4j_fmod<float16>(float16 num, float16 denom) {
            return (float16) fmodf((float) num, (float) denom);
        }



		template<>
        math_def inline double nd4j_round<double>(double val) {
			return round(val);
		}

		template<>
        math_def inline int nd4j_round<int>(int val) {
			return round((float) val);
		}

		template<>
        math_def inline float16 nd4j_sin<float16>(float16 val) {
#ifdef NATIVE_HALFS
            return hsin(val.data);
#else
			return (float16) sinf((float) val);
#endif
		}


		template<>
        math_def inline float nd4j_sin<float>(float val) {
			return sinf(val);
		}

		template<>
        math_def inline double nd4j_sin<double>(double val) {
			return sin(val);
		}

		template<>
        math_def inline int nd4j_sin<int>(int val) {
			return sin((float) val);
		}



		template<>
		math_def inline float16 nd4j_sinh<float16>(float16 val) {
#ifdef NATIVE_HALFS
			return hsin(val.data);
#else
			return (float16) sinh((float) val);
#endif
		}


		template<>
		math_def inline float nd4j_sinh<float>(float val) {
			return sinhf(val);
		}

		template<>
		math_def inline double nd4j_sinh<double>(double val) {
			return sinh(val);
		}

		template<>
		math_def inline int nd4j_sinh<int>(int val) {
			return sinhf((float) val);
		}


		template<>
        math_def inline float16 nd4j_sqrt<float16>(float16 val) {
#ifdef NATIVE_HALFS
            return hsqrt(val.data);
#else
			return (float16) sqrtf((float) val);
#endif
		}


		template<>
        math_def inline float nd4j_sqrt<float>(float val) {
			return sqrtf(val);
		}

		template<>
        math_def inline double nd4j_sqrt<double>(double val) {
			return sqrt(val);
		}

		template<>
        math_def inline int nd4j_sqrt<int>(int val) {
			return sqrtf((float) val);
		}

		template<>
        math_def inline float16 nd4j_tanh<float16>(float16 val) {
			return (float16) tanhf((float) val);
		}


		template<>
        math_def inline float nd4j_tanh<float>(float val) {
			return tanhf(val);
		}

		template<>
        math_def inline double nd4j_tanh<double>(double val) {
			return tanh(val);
		}
		template<>
        math_def inline int nd4j_tanh<int>(int val) {
			return tanhf((float) val);
		}


        template<>
        math_def inline float16 nd4j_tan<float16>(float16 val) {
            return (float16) tanf((float) val);
        }


        template<>
        math_def inline float nd4j_tan<float>(float val) {
            return tanf(val);
        }

        template<>
        math_def inline double nd4j_tan<double>(double val) {
            return tan(val);
        }
        template<>
        math_def inline int nd4j_tan<int>(int val) {
            return tanf((float) val);
        }


        template<>
        math_def inline float16 nd4j_acos<float16>(float16 val) {
			return (float16) acosf((float) val);
		}


		template<>
        math_def inline float nd4j_acos<float>(float val) {
			return acosf(val);
		}

		template<>
        math_def inline double nd4j_acos<double>(double val) {
			return acos(val);
		}

		template<>
        math_def inline int nd4j_acos<int>(int val) {
			return acosf((float) val);
		}


		template<>
        math_def inline float16 nd4j_asin<float16>(float16 val) {
			return (float16) asinf((float) val);
		}


		template<>
        math_def inline float nd4j_asin<float>(float val) {
			return asinf(val);
		}

		template<>
        math_def inline double nd4j_asin<double>(double val) {
			return asin(val);
		}

		template<>
        math_def inline int nd4j_asin<int>(int val) {
			return asinf((float) val);
		}


		template<>
        math_def inline float16 nd4j_atan<float16>(float16 val) {
			return (float16) atanf((float)val);
		}


		template<>
        math_def inline float nd4j_atan<float>(float val) {
			return atanf(val);
		}

		template<>
        math_def inline double nd4j_atan<double>(double val) {
			return atan(val);
		}

		template<>
        math_def inline int nd4j_atan<int>(int val) {
			return atanf((float) val);
		}





        template<>
        math_def inline float16 nd4j_atanh<float16>(float16 val) {
            return (float16) atanhf((float)val);
        }


        template<>
        math_def inline float nd4j_atanh<float>(float val) {
            return atanhf(val);
        }

        template<>
        math_def inline double nd4j_atanh<double>(double val) {
            return atanh(val);
        }

        template<>
        math_def inline int nd4j_atanh<int>(int val) {
            return atanhf((float) val);
        }




        template<typename T>
        math_def inline void nd4j_swap(T &val1, T &val2) {
            T temp = val1; val1=val2; val2=temp;
		};

#ifdef __CUDACC__
		namespace atomics {
template <typename T>
__device__ T nd4j_atomicAdd(T* address, T val);

template <typename T>
__device__ T nd4j_atomicSub(T* address, T val);
template <typename T>
__device__ T nd4j_atomicMul(T* address, T val);
template <typename T>
__device__ T nd4j_atomicDiv(T* address, T val);

template <>
__device__ double nd4j_atomicAdd<double>(double* address, double val)  {
	unsigned long long int* address_as_ull =
			(unsigned long long int *) address;
	unsigned long long int old = *address_as_ull, assumed;
	do {
		assumed = old;
		old = atomicCAS(address_as_ull, assumed,__double_as_longlong(val +
				__longlong_as_double(assumed)));
	} while (assumed != old);
	return __longlong_as_double(old);
}

template <>
__device__ float16 nd4j_atomicAdd<float16>(float16* address, float16 val)  {
	int* address_as_ull = (int*) address;

	long addr = (long) address;
	bool misaligned = addr & 0x3;

	if (misaligned)
		address_as_ull = (int *) (addr - 2);

	PAIR old, assumed, fresh;

	old.W = *address_as_ull;
	do {

		if (!misaligned) {
			float16 res = ((float16) old.B.H) + val;
			fresh.B.H = res.data;
			fresh.B.L = old.B.L;
		} else {
			float16 res = ((float16) old.B.L) + val;
			fresh.B.L = res.data;
			fresh.B.H = old.B.H;
		}

		assumed.W = old.W;
		old.W = atomicCAS(address_as_ull, assumed.W, fresh.W);
	} while (assumed.W != old.W);

	if (!misaligned) return old.B.H;
	else return old.B.L;
}

template <>
__device__ double nd4j_atomicSub<double>(double* address, double val)  {
	unsigned long long int* address_as_ull =
			(unsigned long long int *) address;
	unsigned long long int old = *address_as_ull, assumed;
	do {
		assumed = old;
		old = atomicCAS(address_as_ull, assumed,__double_as_longlong(val -
				__longlong_as_double(assumed)));
	} while (assumed != old);
	return __longlong_as_double(old);
}

template <>
__device__ double nd4j_atomicMul<double>(double* address, double val)  {
	unsigned long long int* address_as_ull =
			(unsigned long long int*) address;
	unsigned long long int old = *address_as_ull, assumed;
	do {
		assumed = old;
		old = atomicCAS(address_as_ull, assumed,__double_as_longlong(val *
				__longlong_as_double(assumed)));
	} while (assumed != old);
	return __longlong_as_double(old);
}

template <>
__device__ double nd4j_atomicDiv<double>(double* address, double val)  {
	unsigned long long int* address_as_ull =
			(unsigned long long int*) address;
	unsigned long long int old = *address_as_ull, assumed;
	do {
		assumed = old;
		old = atomicCAS(address_as_ull, assumed,__double_as_longlong(val /
				__longlong_as_double(assumed)));
	} while (assumed != old);
	return __longlong_as_double(old);
}

template <>
__device__ float nd4j_atomicAdd<float>(float* address, float val)  {
	return atomicAdd(address,val);
}


template <>
__device__ float nd4j_atomicSub<float>(float* address, float val) {
	int* address_as_ull = (int*) address;
	int old = *address_as_ull, assumed;
	do {
		assumed = old;
		old = atomicCAS(address_as_ull, assumed, __float_as_int(val -
				__float_as_int(assumed)));
	} while (assumed != old);
	return __int_as_float(old);
}

template <>
__device__ float nd4j_atomicMul<float>(float* address, float val) {
	int* address_as_ull =
			( int*)address;
	int old = *address_as_ull, assumed;
	do {
		assumed = old;
		old = atomicCAS(address_as_ull, assumed, __float_as_int(val *
				__float_as_int(assumed)));
	} while (assumed != old);
	return __int_as_float(old);
}


template <>
__device__ float nd4j_atomicDiv<float>(float* address, float val) {
	int* address_as_ull =
			(int*)address;
	int old = *address_as_ull, assumed;
	do {
		assumed = old;
		old = atomicCAS(address_as_ull, assumed, __float_as_int(val *
				__float_as_int(assumed)));
	} while (assumed != old);
	return __int_as_float(old);
}
}
#endif
	}

}

#endif /* TEMPLATEMATH_H_ */