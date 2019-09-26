#include <iostream>
#include "QDateTime"
#include <OSGDB/WriteFile>
#include <OSG/Geode>
#include <OSG/Geometry>
#include "OSG/PagedLOD"

#include "QTextStream"

#include "AsciiBlockSpliter.h"
#include "GeneratePldStruct.h"
using namespace DC::Pyra;

AsciiBlockSpliter::AsciiBlockSpliter(const QString& suffix)
	: SPliter(suffix)
{

}

AsciiBlockSpliter::~AsciiBlockSpliter()
{

}

// ���������ļ��ı߽��
ErrorType AsciiBlockSpliter::GetBoundBoxByFile(const QString& inFile, BoundBox& outBox, unsigned simple/* = 1*/)
{
	

	Point3D currentPoint;
	//��ȡ�ļ�,ÿ��һ����洢��currentPoint���������Сֵ�Ƚϣ�ȷ����ֵ
	char currentLine[500];

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Truncate))
	{
		return eError;
	}
	BoundBox box;
	while(file.readLine(currentLine, 500) > 0)
	{

		//�ָ��в���currentPoint��ֵ
		QStringList list = QString(currentLine).split(QRegExp(",|\\s+"),QString::SkipEmptyParts);

		if (list.size() < 3)
		{
			return eError;
		}
		currentPoint.x() = list[0].toDouble();
		currentPoint.y() = list[1].toDouble();
		currentPoint.z() = list[2].toDouble();

		//����boxֵ
		box.m_minCorner[0] = std::min(box.m_minCorner[0], currentPoint[0]);
		box.m_minCorner[1] = std::min(box.m_minCorner[1], currentPoint[1]);
		box.m_minCorner[2] = std::min(box.m_minCorner[2], currentPoint[2]);

		box.m_maxCorner[0] = std::max(box.m_maxCorner[0], currentPoint[0]);
		box.m_maxCorner[1] = std::max(box.m_maxCorner[1], currentPoint[1]);
		box.m_maxCorner[2] = std::max(box.m_maxCorner[2], currentPoint[2]);

		//Ӧ�ò�����
		for (unsigned i = 0; i < simple - 1; ++i)
		{
			file.readLine(currentLine, 500);
		}
	}

	file.close();

	box.m_length = box.m_maxCorner.x() - box.m_minCorner.x();
	box.m_width = box.m_maxCorner.y() - box.m_minCorner.y();
	outBox = box;
	m_boundBox = box;

	std::cout << "finish Box" << std::endl;
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
	std::cout << current_date.toStdString() << std::endl;
	return eNoError;
}

ErrorType AsciiBlockSpliter::WriteSubfile(QString infile, const QDir& outDir, const LevelType& level)
{
	//����box��level,��outfile������ļ�
	//1.����levelȷ�����к�
	UpdataFileMap( infile, outDir,  level);

	//��ȡ�ļ��е�ÿ���㣬���ݵ��������������ķ����ţ������򿪶�Ӧ���ļ�д�����Ϣ
	Point3D currentPoint;
	QFile file(infile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Truncate))
	{
		return eError;
	}

	QTextStream inStream(&file);
	
	try
	{
		while(!inStream.atEnd())
		{
			//�ָ��в���currentPoint��ֵ
			QString line = inStream.readLine();
			QStringList list = line.split(QRegExp(",|\\s+"),QString::SkipEmptyParts);

			if (list.size() < 3)
			{
				return eError;
			}

			currentPoint.x() = list[0].toDouble();
			currentPoint.y() = list[1].toDouble();
			currentPoint.z() = list[2].toDouble();
			//����currentPoint��������Ų�д��

			unsigned columnIndex = 0, rowIndex = 0;
			m_boundBox.GetPointIndexInLevel(currentPoint, level, columnIndex, rowIndex);
			QString indexName = GenIndexNameByIndex(columnIndex, rowIndex);

			//3.��������ļ���
			//QString currentOutFilename = outFile + "\\" + fMap[indexName] + m_extension;
			QString curFilename = QString("%1.%2").arg(m_fMap[indexName]).arg(GetSuffix());
			QString currentOutFilename = outDir.filePath(curFilename);

			//boxfile
			QString curboxFilename = QString("%1_box.%2").arg(m_fMap[indexName]).arg(GetSuffix());
			QString currentBoxOutFilename = outDir.filePath(curboxFilename);

			//����һ����ǰ�б���
			QString writeline;

			//д�뵱ǰ����
			writeline.append(QString("%1").arg(currentPoint.x(), 0, 'f', 3));
			writeline.append(","); //��ӷָ���
			writeline.append(QString("%1").arg(currentPoint.y(), 0, 'f', 3));
			writeline.append(","); //��ӷָ���
			writeline.append(QString("%1").arg(currentPoint.z(), 0, 'f', 3));
			m_fileMaps[currentOutFilename]->m_qdatafile->write(writeline.toStdString().c_str());
			m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");

			}
	}
	catch (...)
	{
		auto aa = 5;
	}

	file.close();
	//�ر�m_fileMaps�е�����file���ر�ǰд������box�ļ�
	for (auto it = m_fileMaps.cbegin(); it != m_fileMaps.cend(); ++it)
	{
		it->second->m_qdatafile->close();
		delete it->second->m_qdatafile;
		it->second->m_qdatafile = nullptr;
		delete it->second->m_box;
		it->second->m_box = nullptr;
	}

	m_fileMaps.clear();
	GeneratePldStruct gPldStruct(m_boundBox, ".dat");
	gPldStruct.WriteOsgBoxToFile(infile, level, outDir);
	std::cout << "finish bootom file" << std::endl;
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
	std::cout << current_date.toStdString() << std::endl;
	return eNoError;
}

void AsciiBlockSpliter::WriteBoxTofile(QString infile, unsigned level, const QDir& outDir)
{
}

void AsciiBlockSpliter::WriteFileByCombine(QString infile, unsigned level, const QDir& outDir, unsigned simple/* = 3*/)
{
	//���ݵײ��ļ��ϲ������ϲ��ļ�
	//1.����levelȷ�����к�
	for (int i = level-1; i != 0; i--)
	{
		QFileInfo info(infile);

		//��һ����ļ���Ϣ
		FileInfoParse fParseDown(info.baseName(),i+1);
		IndexFileMap fMapdown = fParseDown.GetFileMap();

		//��һ����Ϣ
		FileInfoParse fParseUp(info.baseName(),i);
		IndexFileMap fMapUp = fParseUp.GetFileMap();

		//����IndexFileMap�����ڶ�Ӧ��file��д�����ݲ����
		for (auto it = fMapUp.begin(); it != fMapUp.end(); ++it)
		{
			//����keyֵ�����ɶ�Ӧ���ĸ�key
			QStringList fileList = GenIndexsByIndex(i, it->first);

			//�ж��ĸ����ļ������ٴ���һ������Ϊ��Ч���Ŵ����µ�outfile
			bool validSubfile = true;
			for (auto kk = 0; kk != fileList.size(); ++kk)
			{
				QString curName = QString("%1.%2").arg(fMapdown[fileList[kk]]).arg(GetSuffix());
				QString currentinFilename = outDir.filePath(curName);
				QFileInfo info(currentinFilename);
				if (info.exists())
				{
					validSubfile = true;
					break;
				}
				validSubfile = false;
			}

			if (!validSubfile)
			{
				continue;
			}

			//����keyֵ������file����
			QString cName = QString("%1.%2").arg(it->second).arg(GetSuffix());
			QString currentOutFilename = outDir.filePath(cName);
			QFile outFile(currentOutFilename);
			if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				continue;
			}

			//�����ĸ����ļ���������������д�뵽outfile�У�����¼boxֵ�����д��
			BoundBox comBineBox;
			for (int fnum = 0; fnum != 4; ++fnum)
			{
				char currentLine[500];
				QString curName = QString("%1.%2").arg(fMapdown[fileList[fnum]]).arg(GetSuffix());
				QString currentinFilename = outDir.filePath(curName);
				QFile inFile(currentinFilename);
				if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					continue;
				}

				while(inFile.readLine(currentLine, 500) > 0)
				{
					outFile.write(currentLine);
					//����box
					//�ָ��в���currentPoint��ֵ
					QStringList boxlist = QString(currentLine).split(QRegExp(",|\\s+"),QString::SkipEmptyParts);

					Point3D currentPoint;
					currentPoint.x() = boxlist[0].toDouble(); 
					currentPoint.y() = boxlist[1].toDouble();
					currentPoint.z() = boxlist[0].toDouble();

					comBineBox.m_minCorner[0] = std::min(comBineBox.m_minCorner[0], currentPoint[0]);
					comBineBox.m_minCorner[1] = std::min(comBineBox.m_minCorner[1], currentPoint[1]);
					comBineBox.m_minCorner[2] = std::min(comBineBox.m_minCorner[2], currentPoint[2]);

					comBineBox.m_maxCorner[0] = std::max(comBineBox.m_maxCorner[0], currentPoint[0]);
					comBineBox.m_maxCorner[1] = std::max(comBineBox.m_maxCorner[1], currentPoint[1]);
					comBineBox.m_maxCorner[2] = std::max(comBineBox.m_maxCorner[2], currentPoint[2]);

					//Ӧ�ò�����
					for (unsigned i = 0; i < simple-1; ++i)
					{
						inFile.readLine(currentLine, 500);
					}
				}

				inFile.close();

			}
			
			outFile.close();
		}
	}

	std::cout << "finish combine file" << std::endl;
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
	std::cout << current_date.toStdString() << std::endl;
}

QStringList AsciiBlockSpliter::GenIndexsByIndex(int level, QString index)
{
	QStringList list;

	//���к�
	unsigned column = index.split("_").at(0).toInt() * 2;
	unsigned row = index.split("_").at(1).toInt() * 2;

	list.push_back(QString("%1_%2").arg(column).arg(row));
	list.push_back(QString("%1_%2").arg(column+1).arg(row));
	list.push_back(QString("%1_%2").arg(column).arg(row+1));
	list.push_back(QString("%1_%2").arg(column+1).arg(row+1));

	return list;
}

BoundBox AsciiBlockSpliter::GenSubBoxByFileName(const QString& name)
{
	BoundBox box;

	//! ������֪��Ϣ�ҵ��߽�������Сֵ��Ȼ����box����
	//�ж��ļ�����basename���Ƿ����_�ַ�
	QFileInfo fInfo(name);
	QString baseName = fInfo.baseName();

	if (baseName == "changshu_L0_X0_Y0" || baseName == "changshu_L0_X0_Y0.txt")
	{
		auto aa = 5;
	}
	

	bool hasUnderline = baseName.contains("_");
	int underLineNum = baseName.count("_");

	Q_ASSERT(underLineNum == 3);

	//1.ȡ����Lindex, xindex,yindexֵ
	QStringList bnList = baseName.split("_");
	int lIndex = bnList[1].remove(0,1).toInt();
	int xIndex = bnList[2].remove(0,1).toInt();;
	int yIndex = bnList[3].remove(0,1).toInt();;

	int realLevel = lIndex + 1;

	//2.�к�����ȣ�ȷ���к���
	int cr = std::pow(double(2), (realLevel - 1));

	Point_3D minCorner(m_boundBox.m_minCorner)  ;
	minCorner.x() += m_boundBox.m_length / float(cr) * xIndex;
	minCorner.y() += m_boundBox.m_width / float(cr) * yIndex;
	minCorner.z() += 0;
	Point_3D maxCorner = m_boundBox.m_minCorner;
	maxCorner.x() += m_boundBox.m_length / float(cr) * (xIndex + 1);
	maxCorner.y() += m_boundBox.m_width / float(cr) * (yIndex+1);
	maxCorner.z() +=  (m_boundBox.m_maxCorner.z() - m_boundBox.m_minCorner.z());

	box = BoundBox(minCorner, maxCorner);

	return box;
}

void AsciiBlockSpliter::UpdataFileMap(QString infile, const QDir& outDir, const LevelType& level)
{
	QFileInfo info(infile);
	FileInfoParse fParse(info.baseName(),level);
	m_fMap = fParse.GetFileMap();

		unsigned columnNum  = std::pow(double(2),(level-1));
		unsigned rowNum = columnNum;
		for (unsigned columnIndex = 0; columnIndex < columnNum; columnIndex++)
		{
			for (unsigned rowIndex = 0; rowIndex < rowNum; rowIndex++ )
			{
				

				QString indexName = GenIndexNameByIndex(columnIndex, rowIndex);

				//3.��������ļ���
				QString curFilename = QString("%1.%2").arg(m_fMap[indexName]).arg(GetSuffix());
				QString currentOutFilename = outDir.filePath(curFilename);

				//boxfile
				QString curboxFilename = QString("%1_box.%2").arg(m_fMap[indexName]).arg(GetSuffix());
				QString currentBoxOutFilename = outDir.filePath(curboxFilename);

				//�����ڵ�ǰ���ֶ�Ӧ��Qfiletrunk�����򴴽�
				if (m_fileMaps.find(currentOutFilename) == m_fileMaps.end())
				{
					QFile* odatafile = new QFile(currentOutFilename);
					
					//���¼������ֵ
					BoundBox box = CalcBBox(level, columnIndex, rowIndex, m_boundBox);
					BoundBox* outBox = new BoundBox(box.m_minCorner,  box.m_maxCorner);
					m_fileMaps[currentOutFilename] = new FileTrunkInfo(odatafile, nullptr, outBox);
				}

				//��ʱһ�����ڶ�Ӧ������ӳ���Qfiletrunk���󣬵����в���������ļ��Ƿ��Ѿ���
				if (!m_fileMaps[currentOutFilename]->m_qdatafile->isOpen())
				{
					m_fileMaps[currentOutFilename]->m_qdatafile->open(QIODevice::WriteOnly | QIODevice::Text);

				}
			}
	}
	

}

BoundBox AsciiBlockSpliter::CalcBBox(int level, unsigned col, unsigned row, BoundBox mainBox)
{
	BoundBox subBox;
	mainBox.GetSubBoxByIndexInLevel(level, col, row,subBox.m_minCorner, subBox.m_maxCorner );
	return subBox;
}