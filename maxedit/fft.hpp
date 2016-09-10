#ifndef MAX_FFT
#define MAX_FFT

// All theory for this code can be found in this link:
// http://www.drdobbs.com/cpp/a-simple-and-efficient-fft-implementatio/199500857

template<unsigned M, unsigned N, unsigned B, unsigned A>
struct SinCosSeries {
	static double value() {
		return 1 - (A*M_PI / B)*(A*M_PI / B) / M / (M + 1)
			*SinCosSeries<M + 2, N, B, A>::value();
	}
};

template<unsigned N, unsigned B, unsigned A>
struct SinCosSeries<N, N, B, A> {
	static double value() { return 1.; }
};

template<unsigned B, unsigned A, typename T = double>
struct Sin;

template<unsigned B, unsigned A>
struct Sin<B, A, float> {
	static float value() {
		return (A*M_PI / B)*SinCosSeries<2, 24, B, A>::value();
	}
};
template<unsigned B, unsigned A>
struct Sin<B, A, double> {
	static double value() {
		return (A*M_PI / B)*SinCosSeries<2, 34, B, A>::value();
	}
};

template<unsigned B, unsigned A, typename T = double>
struct Cos;

template<unsigned B, unsigned A>
struct Cos<B, A, float> {
	static float value() {
		return SinCosSeries<1, 23, B, A>::value();
	}
};
template<unsigned B, unsigned A>
struct Cos<B, A, double> {
	static double value() {
		return SinCosSeries<1, 33, B, A>::value();
	}
};

template<unsigned N, typename T = double>
class DanielsonLanczos {
	DanielsonLanczos<N / 2, T> next;
public:
	void apply(T* data) {
		next.apply(data);
		next.apply(data + N);

		T wtemp, tempr, tempi, wr, wi, wpr, wpi;
		wtemp = -Sin<N, 1, T>::value();//sin(M_PI / N);//
		wpr = -2.0*wtemp*wtemp;
		wpi = -Sin<N, 2, T>::value();//-sin(2 * M_PI / N);//
		wr = 1.0;
		wi = 0.0;
		for (unsigned i = 0; i<N; i += 2) {
			tempr = data[i + N] * wr - data[i + N + 1] * wi;
			tempi = data[i + N] * wi + data[i + N + 1] * wr;
			data[i + N] = data[i] - tempr;
			data[i + N + 1] = data[i + 1] - tempi;
			data[i] += tempr;
			data[i + 1] += tempi;

			wtemp = wr;
			wr += wr*wpr - wi*wpi;
			wi += wi*wpr + wtemp*wpi;
		}
		//printf("DanielsonLanczos called with N = %d\n", (int)N);
	}
};

template<typename T>
class DanielsonLanczos<4, T> {
public:
	void apply(T* data) {
		T tr = data[2];
		T ti = data[3];
		data[2] = data[0] - tr;
		data[3] = data[1] - ti;
		data[0] += tr;
		data[1] += ti;
		tr = data[6];
		ti = data[7];
		data[6] = data[5] - ti;
		data[7] = tr - data[4];
		data[4] += tr;
		data[5] += ti;

		tr = data[4];
		ti = data[5];
		data[4] = data[0] - tr;
		data[5] = data[1] - ti;
		data[0] += tr;
		data[1] += ti;
		tr = data[6];
		ti = data[7];
		data[6] = data[2] - tr;
		data[7] = data[3] - ti;
		data[2] += tr;
		data[3] += ti;
	}
};

template<typename T>
class DanielsonLanczos<2, T> {
public:
	void apply(T* data) {
		T tr = data[2];
		T ti = data[3];
		data[2] = data[0] - tr;
		data[3] = data[1] - ti;
		data[0] += tr;
		data[1] += ti;
	}
};

template<typename T>
class DanielsonLanczos<1, T> {
public:
	void apply(T* data) { }
};

template<unsigned P,
	typename T = double>
class GFFT {
	enum { N = 1 << P };
	DanielsonLanczos<N, T> recursion;
private:
	void scramble(T* data) {
		// reverse-binary reindexing
		unsigned i, j, m, n;
		n = N << 1;
		T tmp;
		j = 1;
		for (i = 1; i<n; i += 2) {
			if (j>i) {
				tmp = data[j - 1]; data[j - 1] = data[i - 1]; data[i - 1] = tmp; // swap(data[j - 1], data[i - 1]);
				tmp = data[j]; data[j] = data[i]; data[i] = tmp; // swap(data[j], data[i]);
			}
			m = N;
			while (m >= 2 && j>m) {
				j -= m;
				m >>= 1;
			}
			j += m;
		};
	}
public:
	void fft(T* data) {
		scramble(data);
		recursion.apply(data);
	}
};


#endif // MAX_FFT