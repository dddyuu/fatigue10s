// preFcm.h �޸�
#pragma once

#include <QObject>
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>
#include <Eigen/Dense>  // ���Eigen��֧�־�������

class preFcm : public QObject {
    Q_OBJECT
public:
    explicit preFcm(int clusters = 25, double m = 2.0, QObject* parent = nullptr); // Ĭ�Ͼ�������Ϊ25
    void setCenters(const std::vector<std::vector<double>>& centers);
    std::vector<float> computeMembership(const std::vector<float>& sample);
    std::vector<std::vector<double>> getCenters() const;
    Eigen::MatrixXd getMembershipMatrix() const;

    // ���ѵ������
    void train(const std::vector<std::vector<float>>& data, int max_iters = 100, double epsilon = 1e-5);

    // ��Ӿ�������
    std::vector<std::vector<double>> computeFuzzifiedFeatures() const;

private:
    int clusters;  // ������
    double m;      // ģ��ָ��
    std::vector<std::vector<double>> centers;  // ��������
    Eigen::MatrixXd U;  // �����Ⱦ���
    std::vector<std::vector<float>> trainData; // �洢ѵ������
};