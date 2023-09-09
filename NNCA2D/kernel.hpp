#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/src/Core/ArithmeticSequence.h>
#define EIGEN_DONT_PARALLELIZE

const double PI	=	3.1415926535897932384;
#include <map>
struct pts2D {
	double x,y;
};

class kernel {
public:
  double a;
  std::vector<pts2D> particles_X;
	std::vector<pts2D> particles_Y;

	kernel(std::vector<pts2D>& particles_X, std::vector<pts2D>& particles_Y) {
			this->particles_X = particles_X;
			this->particles_Y = particles_Y;
	}

	virtual double getMatrixEntry(const unsigned i, const unsigned j) {
		std::cout << "virtual getInteraction" << std::endl;
		return 0.0;
	}

	Eigen::VectorXd getRow(const int j, std::vector<int> col_indices) {
		int n_cols = col_indices.size();
		Eigen::VectorXd row(n_cols);
    #pragma omp parallel for
    for(int k = 0; k < n_cols; k++) {
        row(k) = this->getMatrixEntry(j, col_indices[k]);
    }
    return row;
  }

  Eigen::VectorXd getCol(const int k, std::vector<int> row_indices) {
		int n_rows = row_indices.size();
    Eigen::VectorXd col(n_rows);
    #pragma omp parallel for
    for (int j=0; j<n_rows; ++j) {
			col(j) = this->getMatrixEntry(row_indices[j], k);
    }
    return col;
  }

  Eigen::MatrixXd getMatrix(std::vector<int> row_indices, std::vector<int> col_indices) {
		int n_rows = row_indices.size();
		int n_cols = col_indices.size();
    Eigen::MatrixXd mat(n_rows, n_cols);
    #pragma omp parallel for collapse(2)
    for (int j=0; j < n_rows; ++j) {
        // #pragma omp parallel for
        for (int k=0; k < n_cols; ++k) {
            mat(j,k) = this->getMatrixEntry(row_indices[j], col_indices[k]);
        }
    }
    return mat;
  }

	Eigen::MatrixXd getMatrix(int row_start_index, int col_start_index, int row_end_index, int col_end_index) {
		Eigen::MatrixXd mat(row_end_index-row_start_index, col_end_index-col_start_index);
		#pragma omp parallel for collapse(2)
		for (int j=row_start_index; j < row_end_index; ++j) {
				// #pragma omp parallel for
				for (int k=col_start_index; k < col_end_index; ++k) {
						mat(j,k) = this->getMatrixEntry(j, k);
				}
		}
		return mat;
	}

  ~kernel() {};
};

class userkernel: public kernel {
public:
	double chargesFunction(const pts2D r) {
		double q = r.x; //user defined
		return q;
	};
	// #ifdef ONEOVERR
	// userkernel(std::vector<pts2D>& particles_X, std::vector<pts2D>& particles_Y): kernel(particles_X, particles_Y) {
	// };

	// #elif LOGR
	userkernel(std::vector<pts2D> particles_X, std::vector<pts2D> particles_Y): kernel(particles_X, particles_Y) {
	};
	// double getMatrixEntry(const unsigned i, const unsigned j) {
	// 	pts2D r1 = particles_X[i];
	// 	pts2D r2 = particles_X[j];
	// 	double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
	// 	double b = 0.01;
	// 	if (R2 < 1e-10) {
	// 		return 0.0;
	// 	}
	// 	else if (R2 < b*b) {
	// 		return 0.5*R2*log(R2)/b/b;
	// 	}
	// 	else {
	// 		return 0.5*log(R2);
	// 	}
	// }

	// double getMatrixEntry(const unsigned i, const unsigned j) {
	// 	pts2D r1 = particles_X[i];
	// 	pts2D r2 = particles_X[j];
	// 	double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
	// 	double R	=	sqrt(R2);
	// 	double b = 1.0;
	// 	return log(1.0+R/b);
	// }

	// double getMatrixEntry(const unsigned i, const unsigned j) {
	// 	pts2D r1 = particles_X[i];
	// 	pts2D r2 = particles_X[j];
	// 	double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
	// 	double R	=	sqrt(R2);
	// 	double b = 1.0;
	// 	return exp(-R*b);
	// }

	// double getMatrixEntry(const unsigned i, const unsigned j) {
	// 	pts2D r1 = particles_X[i];
	// 	pts2D r2 = particles_X[j];
	// 	double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
	// 	double R	=	sqrt(R2);
	// 	double b = 1.0;
	// 	return 1.0/(1.0+(R/b)*(R/b));
	// }

	// double getMatrixEntry(const unsigned i, const unsigned j) {
	// 	pts2D r1 = particles_X[i];
	// 	pts2D r2 = particles_X[j];
	// 	double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
	// 	double R	=	sqrt(R2);
	// 	double b = 1.0;
	// 	return 1.0+(R/b)*(R/b);
	// }

	// double getMatrixEntry(const unsigned i, const unsigned j) {
	// 	pts2D r1 = particles_X[i];
	// 	pts2D r2 = particles_X[j];
	// 	double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
	// 	double R	=	sqrt(R2);
	// 	double b = 1.0;
	// 	return 1.0/sqrt((R/b)*(R/b)+1);
	// }

	// double getMatrixEntry(const unsigned i, const unsigned j) {
	// 	pts2D r1 = particles_X[i];
	// 	pts2D r2 = particles_X[j];
	// 	double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
	// 	double R	=	sqrt(R2);
	// 	double b = 1.0;
	// 	return exp(-(R/b)*(R/b));
	// }

	// double getMatrixEntry(const unsigned i, const unsigned j) {
	// 	pts2D r1 = particles_X[i];
	// 	pts2D r2 = particles_X[j];
	// 	double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
	// 	double R	=	sqrt(R2);
	// 	double b = 1.0;
	// 	return sqrt((R/b)*(R/b)+1);
	// }

	// double getMatrixEntry(const unsigned i, const unsigned j) {
	// 	pts2D r1 = particles_X[i];
	// 	pts2D r2 = particles_X[j];
	// 	double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
	// 	double R	=	sqrt(R2);
	// 		// double b = 0.0001;
	// 	if (R < b) {
	// 		return R/b;
	// 	}
	// 	else {
	// 		return b/R;
	// 	}
	// }

	double getMatrixEntry(const unsigned i, const unsigned j) {
		pts2D r1 = particles_X[i];
		pts2D r2 = particles_X[j];
		double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
		double R = sqrt(R2);
		double b = 0.0001;
		if (R2 < 1e-10) {
			return 0.0;
		}
		else if (R < b) {
			return (R*log(R)-1)/(b*log(b)-1);
		}
		else {
			return log(R)/log(b);
		}
	}

	// double getMatrixEntry(const unsigned i, const unsigned j) {
	// 	pts2D r1 = particles_X[i];
	// 	pts2D r2 = particles_X[j];
	// 	double R2	=	(r1.x-r2.x)*(r1.x-r2.x) + (r1.y-r2.y)*(r1.y-r2.y);
	// 	double R = sqrt(R2);
	// 	if (R==0) {
	// 		return 0;
	// 	}
	// 	else {
	// 		return R*R*log(R);
	// 	}
	// }

	// #endif
	~userkernel() {};
};
