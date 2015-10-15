#ifndef PHOTOSPLINE_BSPLINE_EVAL_H
#define PHOTOSPLINE_BSPLINE_EVAL_H

#include <random>

namespace photospline{
	
template<typename Alloc>
bool splinetable<Alloc>::searchcenters(const double* x, int* centers) const
{
	for (uint32_t i = 0; i < ndim; i++) {
		
		/* Ensure we are actually inside the table. */
		if (x[i] <= knots[i][0] ||
			x[i] > knots[i][nknots[i]-1])
			return (false);
		
		/*
		 * If we're only a few knots in, take the center to be
		 * the nearest fully-supported knot.
		 */
		if (x[i] < knots[i][order[i]]) {
			centers[i] = order[i];
			continue;
		} else if (x[i] >= knots[i][naxes[i]]) {
			centers[i] = naxes[i]-1;
			continue;
		}
		
		uint32_t min = order[i];
		uint32_t max = nknots[i]-2;
		do {
			centers[i] = (max+min)/2;
			
			if (x[i] < knots[i][centers[i]])
				max = centers[i]-1;
			else
				min = centers[i]+1;
		} while (x[i] < knots[i][centers[i]] ||
				 x[i] >= knots[i][centers[i]+1]);
		
		/*
		 * B-splines are defined on a half-open interval. For the
		 * last point of the interval, move center one point to the
		 * left to get the limit of the sum without evaluating
		 * absent basis functions.
		 */
		if (centers[i] == naxes[i])
			centers[i]--;
	}
	
	return (true);
}
	
template<typename Alloc>
void splinetable<Alloc>::pick_eval_funcs(){
	uint32_t constOrder = order[0];
	for (unsigned int j = 1; j < ndim; j++) {
		if (order[j] != constOrder) {
			constOrder = 0;
			break;
		}
	}
	
	switch(constOrder){
#ifdef PHOTOSPLINE_EVAL_TEMPLATES
		case 2:
			switch(ndim){
					//TODO: fix 1D eval
					//case 1: eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<1,2>; break;
				case 2:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<2,2>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<2,2>;
					break;
				case 3:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<3,2>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<3,2>;
					break;
				case 4:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<4,2>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<4,2>;
					break;
				case 5:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<5,2>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<5,2>;
					break;
				case 6:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<6,2>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<6,2>;
					break;
				case 7:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<7,2>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<7,2>;
					break;
				case 8:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<8,2>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<8,2>;
					break;
				default:
					eval_ptr=&splinetable::ndsplineeval_core;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_core;
			}
			break;
		case 3:
			switch(ndim){
					//TODO: fix 1D eval
					//case 1: eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<1,3>; break;
				case 2:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<2,3>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<2,3>;
					break;
				case 3:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<3,3>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<3,3>;
					break;
				case 4:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<4,3>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<4,3>;
					break;
				case 5:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<5,3>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<5,3>;
					break;
				case 6:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<6,3>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<6,3>;
					break;
				case 7:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<7,3>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<7,3>;
					break;
				case 8:
					eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<8,3>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<8,3>;
					break;
				default:
					eval_ptr=&splinetable::ndsplineeval_core;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_core;
			}
			break;
#endif
		default:
			switch(ndim){
#ifdef PHOTOSPLINE_EVAL_TEMPLATES
					//TODO: fix 1D eval
					//case 1: eval_ptr=&splinetable::ndsplineeval_coreD<1>; break;
				case 2:
					eval_ptr=&splinetable::ndsplineeval_coreD<2>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<2>; break;
					break;
				case 3:
					eval_ptr=&splinetable::ndsplineeval_coreD<3>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<3>; break;
					break;
				case 4:
					eval_ptr=&splinetable::ndsplineeval_coreD<4>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<4>; break;
					break;
				case 5:
					eval_ptr=&splinetable::ndsplineeval_coreD<5>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<5>; break;
					break;
				case 6:
					eval_ptr=&splinetable::ndsplineeval_coreD<6>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<6>; break;
					break;
				case 7:
					eval_ptr=&splinetable::ndsplineeval_coreD<7>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<7>; break;
					break;
				case 8:
					eval_ptr=&splinetable::ndsplineeval_coreD<8>;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<8>; break;
					break;
#endif
				default:
					eval_ptr=&splinetable::ndsplineeval_core;
					v_eval_ptr=&splinetable::ndsplineeval_multibasis_core;
			}
	}
}

template<typename Alloc>
double splinetable<Alloc>::ndsplineeval_core(const int* centers, int maxdegree, detail::buffer2d<float> localbasis) const
{
	uint32_t n;
	float basis_tree[ndim+1];
	int decomposedposition[ndim];
	
	int64_t tablepos = 0;
	for (n = 0; n < ndim; n++) {
		decomposedposition[n] = 0;
		tablepos += (centers[n] - (int64_t)order[n])*(int64_t)strides[n];
	}
	
	basis_tree[0] = 1;
	for (n = 0; n < ndim; n++)
		basis_tree[n+1] = basis_tree[n]*localbasis[n][0];
	uint32_t nchunks = 1;
	for (n = 0; n < ndim - 1; n++)
		nchunks *= (order[n] + 1);
	
	float result = 0;
	n = 0;
	while (true) {
		for (uint32_t i = 0; __builtin_expect(i < order[ndim-1] + 1, 1); i++) {
			result += basis_tree[ndim-1]*
			localbasis[ndim-1][i]*
			coefficients[tablepos + i];
		}
		
		if (__builtin_expect(++n == nchunks, 0))
			break;
		
		tablepos += strides[ndim-2];
		decomposedposition[ndim-2]++;
		
		// Carry to higher dimensions
		uint32_t i;
		for (i = ndim-2;
			 decomposedposition[i] > order[i]; i--) {
			decomposedposition[i-1]++;
			tablepos += (strides[i-1]
						 - decomposedposition[i]*strides[i]);
			decomposedposition[i] = 0;
		}
		for (uint32_t j = i; __builtin_expect(j < ndim-1, 1); j++)
			basis_tree[j+1] = basis_tree[j]*
			localbasis[j][decomposedposition[j]];
	}
	
	return result;
}

template<typename Alloc>
template<unsigned int D>
double splinetable<Alloc>::ndsplineeval_coreD(const int* centers, int maxdegree, detail::buffer2d<float> localbasis) const
{
	uint32_t n;
	float basis_tree[D+1];
	int decomposedposition[D];
	
	int64_t tablepos = 0;
	for (n = 0; n < D; n++) {
		decomposedposition[n] = 0;
		tablepos += (centers[n] - (int64_t)order[n])*(int64_t)strides[n];
	}
	
	basis_tree[0] = 1;
	for (n = 0; n < D; n++)
		basis_tree[n+1] = basis_tree[n]*localbasis[n][0];
	uint32_t nchunks = 1;
	for (n = 0; n < D - 1; n++)
		nchunks *= (order[n] + 1);
	
	float result = 0;
	for(uint32_t n=0; __builtin_expect(n<(nchunks-1),1); n++){
		for (uint32_t i = 0; i < (order[D-1] + 1); i++)
			result+=basis_tree[D-1]*localbasis[D-1][i]*coefficients[tablepos+i];
		
		tablepos += strides[(int)D-2];
		decomposedposition[(int)D-2]++;
		
		// Carry to higher dimensions
		uint32_t i;
		for (i = D-2; decomposedposition[i] > order[i]; i--) {
			decomposedposition[i-1]++;
			tablepos += (strides[i-1] - decomposedposition[i]*strides[i]);
			decomposedposition[i] = 0;
		}
		for (uint32_t j = i; __builtin_expect(j < D-1, 1); j++)
			basis_tree[j+1] = basis_tree[j]*
			localbasis[j][decomposedposition[j]];
	}
	for (uint32_t i = 0; i < (order[D-1] + 1); i++)
		result+=basis_tree[D-1]*localbasis[D-1][i]*coefficients[tablepos+i];
	
	return result;
}

template<typename Alloc>
template<unsigned int D, unsigned int O>
double splinetable<Alloc>::ndsplineeval_coreD_FixedOrder(const int* centers, int maxdegree, detail::buffer2d<float> localbasis) const
{
	uint32_t n;
	float basis_tree[D+1];
	int decomposedposition[D];
	
	int64_t tablepos = 0;
	for (n = 0; n < D; n++) {
		decomposedposition[n] = 0;
		tablepos += (centers[n] - (int64_t)O)*(int64_t)strides[n];
	}
	
	basis_tree[0] = 1;
	for (n = 0; n < D; n++)
		basis_tree[n+1] = basis_tree[n]*localbasis[n][0];
	uint32_t nchunks = 1;
	for (n = 0; n < D - 1; n++)
		nchunks *= (O + 1);
	
	float result = 0;
	for(uint32_t n=0; __builtin_expect(n<(nchunks-1),1); n++){
		for (uint32_t i = 0; i < (O + 1); i++)
			result+=basis_tree[D-1]*localbasis[D-1][i]*coefficients[tablepos+i];
		
		tablepos += strides[(int)D-2];
		decomposedposition[(int)D-2]++;
		
		// Carry to higher dimensions
		uint32_t i;
		for (i = D-2; decomposedposition[i] > O; i--) {
			decomposedposition[i-1]++;
			tablepos += (strides[i-1] - decomposedposition[i]*strides[i]);
			decomposedposition[i] = 0;
		}
		for (uint32_t j = i; __builtin_expect(j < D-1, 1); j++)
			basis_tree[j+1] = basis_tree[j]*
			localbasis[j][decomposedposition[j]];
	}
	for (uint32_t i = 0; i < (O + 1); i++)
		result+=basis_tree[D-1]*localbasis[D-1][i]*coefficients[tablepos+i];
	
	return result;
}

template<typename Alloc>
double splinetable<Alloc>::ndsplineeval(const double* x, const int* centers, int derivatives) const
{
	uint32_t maxdegree = *std::max_element(order,order+ndim) + 1;
	float localbasis_store[ndim*maxdegree];
	detail::buffer2d<float> localbasis{localbasis_store,maxdegree};
	
	for (uint32_t n = 0; n < ndim; n++) {
		if (derivatives & (1 << n)) {
			bspline_deriv_nonzero(&knots[n][0],
								  nknots[n], x[n], centers[n],
								  order[n], localbasis[n]);
		} else {
			bsplvb_simple(&knots[n][0], nknots[n],
						  x[n], centers[n], order[n] + 1,
						  localbasis[n]);
		}
	}
	
	return(ndsplineeval_core(centers, maxdegree, localbasis));
}
	
template<typename Alloc>
double splinetable<Alloc>::ndsplineeval(const double* x, const int* centers, int derivatives, const fast_evaluation_token& token) const
{
	uint32_t maxdegree = *std::max_element(order,order+ndim) + 1;
	float localbasis_store[ndim*maxdegree];
	detail::buffer2d<float> localbasis{localbasis_store,maxdegree};
	
	for (uint32_t n = 0; n < ndim; n++) {
		if (derivatives & (1 << n)) {
			bspline_deriv_nonzero(&knots[n][0],
								  nknots[n], x[n], centers[n],
								  order[n], localbasis[n]);
		} else {
			bsplvb_simple(&knots[n][0], nknots[n],
						  x[n], centers[n], order[n] + 1,
						  localbasis[n]);
		}
	}
	
	return((this->*(token.eval_ptr))(centers, maxdegree, localbasis));
}
	
template<typename Alloc>
typename splinetable<Alloc>::fast_evaluation_token
splinetable<Alloc>::get_evaluation_token() const{
	fast_evaluation_token token;
	
	uint32_t constOrder = order[0];
	for (unsigned int j = 1; j < ndim; j++) {
		if (order[j] != constOrder) {
			constOrder = 0;
			break;
		}
	}
	
	switch(constOrder){
#ifdef PHOTOSPLINE_EVAL_TEMPLATES
		case 2:
			switch(ndim){
					//TODO: fix 1D eval
					//case 1: eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<1,2>; break;
				case 2:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<2,2>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<2,2>;
					break;
				case 3:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<3,2>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<3,2>;
					break;
				case 4:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<4,2>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<4,2>;
					break;
				case 5:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<5,2>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<5,2>;
					break;
				case 6:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<6,2>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<6,2>;
					break;
				case 7:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<7,2>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<7,2>;
					break;
				case 8:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<8,2>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<8,2>;
					break;
				default:
					token.eval_ptr=&splinetable::ndsplineeval_core;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_core;
			}
			break;
		case 3:
			switch(ndim){
					//TODO: fix 1D eval
					//case 1: eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<1,3>; break;
				case 2:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<2,3>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<2,3>;
					break;
				case 3:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<3,3>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<3,3>;
					break;
				case 4:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<4,3>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<4,3>;
					break;
				case 5:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<5,3>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<5,3>;
					break;
				case 6:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<6,3>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<6,3>;
					break;
				case 7:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<7,3>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<7,3>;
					break;
				case 8:
					token.eval_ptr=&splinetable::ndsplineeval_coreD_FixedOrder<8,3>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD_FixedOrder<8,3>;
					break;
				default:
					token.eval_ptr=&splinetable::ndsplineeval_core;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_core;
			}
			break;
#endif
		default:
			switch(ndim){
#ifdef PHOTOSPLINE_EVAL_TEMPLATES
					//TODO: fix 1D eval
					//case 1: eval_ptr=&splinetable::ndsplineeval_coreD<1>; break;
				case 2:
					token.eval_ptr=&splinetable::ndsplineeval_coreD<2>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<2>; break;
					break;
				case 3:
					token.eval_ptr=&splinetable::ndsplineeval_coreD<3>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<3>; break;
					break;
				case 4:
					token.eval_ptr=&splinetable::ndsplineeval_coreD<4>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<4>; break;
					break;
				case 5:
					token.eval_ptr=&splinetable::ndsplineeval_coreD<5>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<5>; break;
					break;
				case 6:
					token.eval_ptr=&splinetable::ndsplineeval_coreD<6>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<6>; break;
					break;
				case 7:
					token.eval_ptr=&splinetable::ndsplineeval_coreD<7>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<7>; break;
					break;
				case 8:
					token.eval_ptr=&splinetable::ndsplineeval_coreD<8>;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_coreD<8>; break;
					break;
#endif
				default:
					token.eval_ptr=&splinetable::ndsplineeval_core;
					token.v_eval_ptr=&splinetable::ndsplineeval_multibasis_core;
			}
	}
	return(token);
}

template<typename Alloc>
double splinetable<Alloc>::ndsplineeval_deriv2(const double* x, const int* centers, int derivatives) const
{
	uint32_t maxdegree = *std::max_element(order,order+ndim) + 1;
	float localbasis_store[ndim*maxdegree];
	detail::buffer2d<float> localbasis{localbasis_store,maxdegree};
	
	for (uint32_t n = 0; n < ndim; n++) {
		if (derivatives & (1 << n)) {
			for (int32_t i = 0; i <= order[n]; i++)
				localbasis[n][i] = bspline_deriv_2(
												   &knots[n][0], x[n],
												   centers[n] - order[n] + i, 
												   order[n]);
		} else {
			bsplvb_simple(&knots[n][0], nknots[n],
						  x[n], centers[n], order[n] + 1,
						  localbasis[n]);
		}
	}
	
	return ndsplineeval_core(centers, maxdegree, localbasis);
}
	
template<typename Alloc>
typename splinetable<Alloc>::benchmark_results
splinetable<Alloc>::benchmark_evaluation(size_t trialCount, bool verbose){
	std::mt19937 rng;
	
	volatile double dummy;
	benchmark_results result;
	fast_evaluation_token token=get_evaluation_token();
	
	std::vector<std::uniform_real_distribution<>> dists;
	for(size_t i=0; i<ndim; i++)
		dists.push_back(std::uniform_real_distribution<>(lower_extent(i),upper_extent(i)));
	
	std::vector<double> coords(ndim);
	std::vector<int> centers(ndim);
	std::vector<double> gradeval(ndim+1);
	std::chrono::high_resolution_clock::time_point t1, t2;
	
	rng.seed(52);
	t1 = std::chrono::high_resolution_clock::now();
	for(size_t i=0; i<trialCount; i++){
		for(size_t j=0; j<ndim; j++)
			coords[j]=dists[j](rng);
		
		if(!searchcenters(coords.data(), centers.data()))
			throw std::logic_error("center lookup failure for pint which should be in bounds");
		
		dummy=ndsplineeval(coords.data(), centers.data(), 0, token);
	}
	t2 = std::chrono::high_resolution_clock::now();
	result.single_eval_rate=trialCount/
	  std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
	
	rng.seed(52);
	t1 = std::chrono::high_resolution_clock::now();
	for(size_t i=0; i<trialCount; i++){
		for(size_t j=0; j<ndim; j++)
			coords[j]=dists[j](rng);
		
		if(!searchcenters(coords.data(), centers.data()))
			throw std::logic_error("center lookup failure for pint which should be in bounds");
		
		dummy=ndsplineeval(coords.data(), centers.data(), 0);
		for(size_t j=0; j<ndim; j++)
			dummy=ndsplineeval(coords.data(), centers.data(), 1u<<j, token);
	}
	t2 = std::chrono::high_resolution_clock::now();
	result.gradient_single_eval_rate=trialCount/
	std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
	
	rng.seed(52);
	t1 = std::chrono::high_resolution_clock::now();
	for(size_t i=0; i<trialCount; i++){
		for(size_t j=0; j<ndim; j++)
			coords[j]=dists[j](rng);
		
		if(!searchcenters(coords.data(), centers.data()))
			throw std::logic_error("center lookup failure for pint which should be in bounds");
		
		ndsplineeval_gradient(coords.data(), centers.data(), gradeval.data(), token);
	}
	t2 = std::chrono::high_resolution_clock::now();
	result.gradient_multi_eval_rate=trialCount/
	std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
	
	if(verbose){
		printf("Benchmark results:\n");
		printf("\t%lf single evaluations/second\n",result.single_eval_rate);
		printf("\t%lf 'single' gradient evaluations/second\n",result.gradient_single_eval_rate);
		printf("\t%lf 'multiple' gradient evaluations/second\n",result.gradient_multi_eval_rate);
		printf("\t(%zu trial evaluations)\n",trialCount);
	}
	
	return(result);
}
	


} //namespace photospline

#endif
