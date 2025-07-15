// preFcm.cpp �޸�
#include "preFcm.h"
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <random>
#include <Eigen/Dense>

preFcm::preFcm(int clusters, double m, QObject* parent)
    : QObject(parent), clusters(clusters), m(m), U(Eigen::MatrixXd::Zero(clusters, 0)) {
    // ��ʼ�������������
    std::random_device rd;
    std::mt19937 gen(rd());
}

void preFcm::setCenters(const std::vector<std::vector<double>>& centers) {
    if (centers.size() != static_cast<size_t>(clusters)) {
        throw std::invalid_argument("��������������ƥ��");
    }
    this->centers = centers;
}

std::vector<std::vector<double>> preFcm::getCenters() const {
    return centers;
}

Eigen::MatrixXd preFcm::getMembershipMatrix() const {
    return U;
}

void preFcm::train(const std::vector<std::vector<float>>& data, int max_iters, double epsilon) {
    if (data.empty()) return;

    const int n = data.size();       // ������
    const int dim = data[0].size();  // ����ά��

    trainData = data;
    U = Eigen::MatrixXd::Zero(clusters, n);

    // 1. �����ʼ�������Ⱦ���
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < clusters; ++i) {
        for (int k = 0; k < n; ++k) {
            U(i, k) = dis(gen);
        }
    }

    // ��һ��ʹÿ�к�Ϊ1
    for (int k = 0; k < n; ++k) {
        double sum = 0.0;
        for (int i = 0; i < clusters; ++i) {
            sum += U(i, k);
        }
        for (int i = 0; i < clusters; ++i) {
            U(i, k) /= sum;
        }
    }

    // �����Ż�
    Eigen::MatrixXd prevU;
    for (int iter = 0; iter < max_iters; ++iter) {
        prevU = U;

        // 2. ���¾�������
        centers.clear();
        centers.resize(clusters, std::vector<double>(dim, 0.0));

        for (int i = 0; i < clusters; ++i) {
            double denominator = 0.0;
            std::vector<double> numerator(dim, 0.0);

            for (int k = 0; k < n; ++k) {
                double um = std::pow(U(i, k), m);
                denominator += um;

                for (int d = 0; d < dim; ++d) {
                    numerator[d] += um * data[k][d];
                }
            }

            for (int d = 0; d < dim; ++d) {
                centers[i][d] = numerator[d] / denominator;
            }
        }

        // 3. ����������
        for (int i = 0; i < clusters; ++i) {
            for (int k = 0; k < n; ++k) {
                double dist_ik = 0.0;
                for (int d = 0; d < dim; ++d) {
                    double diff = data[k][d] - centers[i][d];
                    dist_ik += diff * diff;
                }
                dist_ik = std::sqrt(dist_ik);

                double sum = 0.0;
                for (int j = 0; j < clusters; ++j) {
                    double dist_jk = 0.0;
                    for (int d = 0; d < dim; ++d) {
                        double diff = data[k][d] - centers[j][d];
                        dist_jk += diff * diff;
                    }
                    dist_jk = std::sqrt(dist_jk);

                    if (dist_jk < 1e-10) {
                        sum = (i == j) ? 1.0 : 0.0;
                        break;
                    }
                    sum += std::pow(dist_ik / dist_jk, 2.0 / (m - 1.0));
                }

                U(i, k) = 1.0 / sum;
            }
        }

        // 4. �����������
        double max_diff = (U - prevU).cwiseAbs().maxCoeff();
        if (max_diff < epsilon) {
            break;
        }
    }
}

std::vector<float> preFcm::computeMembership(const std::vector<float>& sample) {
    if (centers.empty()) {
        throw std::runtime_error("��������δ��ʼ��");
    }

    std::vector<float> membership(clusters, 0.0f);
    std::vector<double> distances(clusters, 0.0);

    // ���㵽���������ĵľ���
    for (int i = 0; i < clusters; ++i) {
        double dist = 0.0;
        for (size_t j = 0; j < sample.size(); ++j) {
            double diff = static_cast<double>(sample[j]) - centers[i][j];
            dist += diff * diff;
        }
        distances[i] = (dist < 1e-10) ? 1e-10 : std::sqrt(dist);
    }

    // ����������
    for (int i = 0; i < clusters; ++i) {
        double sum = 0.0;
        for (int k = 0; k < clusters; ++k) {
            sum += std::pow(distances[i] / distances[k], 2.0 / (m - 1.0));
        }
        membership[i] = static_cast<float>(1.0 / sum);
    }

    return membership;
}

std::vector<std::vector<double>> preFcm::computeFuzzifiedFeatures() const {
    if (centers.empty() || U.size() == 0) {
        throw std::runtime_error("ģ��δѵ��");
    }

    // ����������תΪEigen���� (clusters x dim)
    Eigen::MatrixXd C(clusters, centers[0].size());
    for (int i = 0; i < clusters; ++i) {
        for (size_t j = 0; j < centers[i].size(); ++j) {
            C(i, j) = centers[i][j];
        }
    }

    // ����ģ��������: X = (C * U)^T
    Eigen::MatrixXd X = (C * U).transpose();

    // תΪ��׼vector
    std::vector<std::vector<double>> fuzzifiedFeatures;
    fuzzifiedFeatures.reserve(X.rows());

    for (int i = 0; i < X.rows(); ++i) {
        std::vector<double> row;
        row.reserve(X.cols());
        for (int j = 0; j < X.cols(); ++j) {
            row.push_back(X(i, j));
        }
        fuzzifiedFeatures.push_back(row);
    }

    return fuzzifiedFeatures;
}