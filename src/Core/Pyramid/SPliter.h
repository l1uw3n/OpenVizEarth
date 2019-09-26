#ifndef SPLITER_H
#define SPLITER_H

#include "QDir"
#include <map>

#include "Common.h"
#include "BoundBox.h"

namespace DC
{
	namespace Pyra
	{
		typedef std::map<QString, QString> IndexFileMap;
		class FileInfoParse
		{
		public: 
			FileInfoParse(QString baseName, const LevelType& level)
				: m_level(level)
				, m_baseName(baseName)
			{
				Parse();
			}

			//����������Ϣ
			void Parse()
			{
				m_totalChunk = std::pow(double(4), (m_level - 1));

				m_chunkColumn = std::pow(double(2), (m_level - 1));;
				m_chunkRow = m_chunkColumn;

				//�����ļ���
				GenFileName();
			}

			//�����ļ���
			void GenFileName()
			{
				//�ȸ�ÿһ�и�ֵ
				for (unsigned column = 0; column < m_chunkColumn; ++column)
				{
					for (unsigned row = 0; row < m_chunkColumn; ++row)
					{
						//���к�����
						QString cIndex = GenIndexNameByIndex(column, row);
						QString fName = m_baseName + QString("_L%1_X%2_Y%3").arg(m_level-1).arg(column).arg(row);
						m_indexToFile[cIndex] = fName;
					}
				}
			}

			IndexFileMap GetFileMap()
			{
				return m_indexToFile;
			}
		public:
			//�㼶��
			LevelType m_level;

			//basename
			QString m_baseName;

			//�ֿ����
			unsigned m_totalChunk;

			//������
			unsigned m_chunkColumn;
			unsigned m_chunkRow;

			//���к�������Ӧ���ļ���
			IndexFileMap m_indexToFile;
		};

		class SPliter
		{
		public:
			SPliter(const QString& suffix);
			~SPliter();

			//
			QString GetSuffix() {return m_suffix;}

			//��ȡ�ļ���ȷ���߽�з�Χ
			virtual	ErrorType GetBoundBoxByFile(const QString& inFile, BoundBox& outBox, unsigned simple = 1) = 0;

			//����������д���ֿ��ļ�
			virtual ErrorType WriteSubfile(QString infile, const QDir& outDir, const LevelType& level) = 0;

			//������ײ����ݣ��ϲ�����ϡ����������
			virtual void WriteFileByCombine(QString infile, unsigned level, const QDir& outDir, unsigned simple = 3) = 0;

			//д���߽��
			virtual void WriteBoxTofile(QString infile, unsigned level, const QDir& outDir) = 0;
		private:
			QString m_suffix;
		};
	}
}



#endif // SPLITER_H
