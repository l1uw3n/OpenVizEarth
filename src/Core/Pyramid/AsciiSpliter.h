#ifndef ASCIISPLITER_H
#define ASCIISPLITER_H

#include "QMap"
#include "QStringList"
#include "SPliter.h"

namespace DC
{
	namespace Pyra
	{
		struct FileTrunkInfo
		{
			FileTrunkInfo()
				: m_box(nullptr)
				, m_qdatafile(nullptr)
			{

			}

			FileTrunkInfo(QFile* qdatafile, QFile* qBoxfile, BoundBox* box)
				: m_box(nullptr)
				, m_qdatafile(nullptr)
			{
				m_box = box;
				m_qdatafile = qdatafile;
			}

			BoundBox* m_box;
			QFile* m_qdatafile;
		};

		class AsciiSpliter : public SPliter
		{
		public:
			AsciiSpliter(const QString& suffix);
			~AsciiSpliter();

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

			//! д�ļ�ͷ
			void WriteHeader(QString& basename, QString& fullName);
		
		private:
			BoundBox m_boundBox;

			//Qfile�ļ���������
			std::map<QString, FileTrunkInfo* > m_fileMaps;
		};

	}
}

#endif // ASCIISPLITER_H
