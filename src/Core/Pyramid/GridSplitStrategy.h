#ifndef GRIDSPLITSTRATEGY_H
#define GRIDSPLITSTRATEGY_H

#include <map>
#include "QMap"
#include "QFile"

#include "pyramidDLL.h"
#include "FileSplitStrategy.h"

#include "BoundBox.h"

namespace DC
{
	namespace Pyra
	{
		class PYRAMID_API GridSplitStrategy : public FileSplitStrategy
		{
		public:
			GridSplitStrategy();
			~GridSplitStrategy();

			//�ļ��ָ�ӿں���
			ErrorType Spit(const QString& inFile, const LevelType& level, const QDir& outDir, bool isAll = true);

			//*****************************Spit����㷨begin*******************************************************
			void SetBoxSample(unsigned sample);
			void SetLevelSample(unsigned sample);
			
			//*****************************Spit����㷨end*******************************************************
		private:
			//�ļ������ݱ߽��
			BoundBox m_bound;

			//�ֲ㼶��
			LevelType m_level;

			//���ָ���ļ���
			QString m_inFile;

			//��չ��
			QString m_extension;

			//��ȡ�ļ���ȡboxʱ�������
			unsigned m_boxSample;

			//�ϲ�ʱ�������
			unsigned m_levelSample;
		};
	}
}

#endif // GRIDSPLITSTRATEGY_H
