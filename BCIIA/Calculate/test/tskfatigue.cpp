#include "tskfatigue.h"
#include <iostream>
#include <random>
int main()
{
	Calculate::TskFatigue tskfatigue;
	std::vector<float> input;
	std::random_device rd;  // �����������ɵ�����豸
	std::mt19937 gen(rd()); // ʹ������豸��ʼ�� Mersenne Twister ����

	// ����һ�����ȷֲ��ĸ���������������Χ�� 0 �� 1 ֮��
	std::uniform_real_distribution<> dis(0.0, 1.0);

	// ����һ�� 0 �� 1 ֮��������
	
	for (int i = 0; i < 25; i++)
	{
		double random_value = dis(gen);
		input.push_back(random_value);
	}
	tskfatigue.run(input);
	std::vector<std::vector<float>>  output =tskfatigue.getOutputValue();
	for (int i = 0; i < output.size(); i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::cout << output[i][j] << " ";
		}
		std::cout << std::endl;
	}
}