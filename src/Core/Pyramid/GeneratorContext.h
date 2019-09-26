#ifndef PYRAMID_H
#define PYRAMID_H

//Qt
#include "QString"
#include "QDir"

#include "Common.h"
#include "pyramidDLL.h"

namespace DC
{
	namespace Pyra
	{
		//�洢һ�����Զ���
		class FileSplitStrategy;
		class PYRAMID_API GeneratorContext
		{
		public:
			GeneratorContext(const QString& inFile, const LevelType& level, const QDir& outDir, bool isAll = true);
			~GeneratorContext();
			
			//��̬���ĵ�ǰά���Ĳ���
			void SetSplitStrategy(FileSplitStrategy* stratery);

			//���ɽ������ṹ
			ErrorType Generate();
		//��Ա����
		private:
			//�ֲ㼶��
			LevelType m_level;

			//���ָ���ļ���
			QString m_inFile;

			//���·��
			QDir m_outDir;

			//�ָ�����
			bool m_allspit;

			//���Զ���-�ӿ���
			FileSplitStrategy* m_splitStrategy;
		};

	}
}

#endif // PYRAMID_H
