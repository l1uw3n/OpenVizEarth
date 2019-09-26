#ifndef LASSPLITER_H
#define LASSPLITER_H

#include "QMap"
#include "QStringList"
#include "SPliter.h"

namespace DC
{
	namespace Pyra
	{
		class LasSpliter : public SPliter
		{
		public:
			LasSpliter(const QString& suffix);
			~LasSpliter();

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

			ErrorType WriteFileByBoundBox(const QString& infile, const  QString& outFilename, BoundBox box);
		private:
			BoundBox m_boundBox;

			//Qfile�ļ���������
			QMap<QString, QFile* > m_fileMaps;
		};

	}
}

#endif // LASSPLITER_H
