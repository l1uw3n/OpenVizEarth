#ifndef ASCIIBLOCKSPLITER_H
#define ASCIIBLOCKSPLITER_H

#include "QMap"
#include "QStringList"
#include "SPliter.h"
#include "AsciiSpliter.h"

namespace DC
{
	namespace Pyra
	{
		class AsciiBlockSpliter : public SPliter
		{
		public:
			AsciiBlockSpliter(const QString& suffix);
			~AsciiBlockSpliter();

			//��ȡ�ļ���ȷ���߽�з�Χ
			virtual	ErrorType GetBoundBoxByFile(const QString& inFile, BoundBox& outBox, unsigned simple = 1);

			//����������д���ֿ��ļ�
			virtual ErrorType WriteSubfile(QString infile, const QDir& outDir, const LevelType& level);

			//������ײ����ݣ��ϲ�����ϡ����������
			virtual void WriteFileByCombine(QString infile, unsigned level, const QDir& outDir, unsigned simple = 3);

			//д���߽��
			virtual void WriteBoxTofile(QString infile, unsigned level, const QDir& outDir);
		private:
			//��������ֵ�����ĸ��ӿ������ֵ
			QStringList GenIndexsByIndex(int level, QString index);

			//! �����nameΪȫ·������
			BoundBox GenSubBoxByFileName(const QString& name);

			void UpdataFileMap(QString infile, const QDir& outDir, const LevelType& level);
			
			//�� �� �� �� �߽� �����Ӧ�ı߽�
			BoundBox CalcBBox(int level, unsigned col, unsigned row, BoundBox mainBox);

		private:
			BoundBox m_boundBox;

			//Qfile�ļ���������
			std::map<QString, FileTrunkInfo* > m_fileMaps;
			IndexFileMap m_fMap;
		};

	}
}

#endif // ASCIISPLITER_H
