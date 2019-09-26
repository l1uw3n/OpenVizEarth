#include <iostream>
#include "QDateTime"

#include "QTextStream"

#include "AsciiSpliter.h"

using namespace DC::Pyra;

AsciiSpliter::AsciiSpliter(const QString& suffix)
	: SPliter(suffix)
{

}

AsciiSpliter::~AsciiSpliter()
{

}

ErrorType AsciiSpliter::GetBoundBoxByFile(const QString& inFile, BoundBox& outBox, unsigned simple/* = 1*/)
{
	BoundBox box;

	Point3D currentPoint;
	//��ȡ�ļ�,ÿ��һ����洢��currentPoint���������Сֵ�Ƚϣ�ȷ����ֵ
	char currentLine[500];

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Truncate))
	{
		return eError;
	}

	while(file.readLine(currentLine, 500) > 0)
	{

		//�ָ��в���currentPoint��ֵ
		QStringList list = QString(currentLine).split(QRegExp(",|\\s+"),QString::SkipEmptyParts);

		if (list.size() < 3)
		{
			return eError;
		}

		//currentPoint = Point3D(list[0].toDouble(), list[1].toDouble(), list[2].toDouble());
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

ErrorType AsciiSpliter::WriteSubfile(QString infile, const QDir& outDir, const LevelType& level)
{
	//����box��level,��outfile������ļ�
	//1.����levelȷ�����к�
	QFileInfo info(infile);
	FileInfoParse fParse(info.baseName(),level);
	IndexFileMap fMap = fParse.GetFileMap();


	//��ȡ�ļ��е�ÿ���㣬���ݵ��������������ķ����ţ������򿪶�Ӧ���ļ�д�����Ϣ
	/*int colCellNum =  std::pow(double(2),(level-1));
	double xInter = m_bound.m_length / colCellNum;
	double yInter = m_bound.m_width / colCellNum;*/

	char currentLine[500];
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

		//	currentPoint = Point3D(list[0].toDouble(), list[1].toDouble(), list[2].toDouble());
			currentPoint.x() = list[0].toDouble();
			currentPoint.y() = list[1].toDouble();
			currentPoint.z() = list[2].toDouble();
			//����currentPoint��������Ų�д��

			unsigned columnIndex = 0, rowIndex = 0;
			m_boundBox.GetPointIndexInLevel(currentPoint, level, columnIndex, rowIndex);
			QString indexName = GenIndexNameByIndex(columnIndex, rowIndex);

			//3.��������ļ���
			//QString currentOutFilename = outFile + "\\" + fMap[indexName] + m_extension;
			QString curFilename = QString("%1.%2").arg(fMap[indexName]).arg(GetSuffix());
			QString currentOutFilename = outDir.filePath(curFilename);

			//boxfile
			QString curboxFilename = QString("%1_box.%2").arg(fMap[indexName]).arg(GetSuffix());
			QString currentBoxOutFilename = outDir.filePath(curboxFilename);

			//�����ڵ�ǰ���ֶ�Ӧ��Qfiletrunk�����򴴽�
			if (m_fileMaps.find(currentOutFilename) == m_fileMaps.end())
			{
				QFile* odatafile = new QFile(currentOutFilename);
				//QFile* oboxfile = new QFile(currentBoxOutFilename);
				BoundBox* outBox = new BoundBox;
				m_fileMaps[currentOutFilename] = new FileTrunkInfo(odatafile, nullptr, outBox);
			}

			//��ʱһ�����ڶ�Ӧ������ӳ���Qfiletrunk���󣬵����в���������ļ��Ƿ��Ѿ���
			if (!m_fileMaps[currentOutFilename]->m_qdatafile->isOpen())
			{
				m_fileMaps[currentOutFilename]->m_qdatafile->open(QIODevice::WriteOnly | QIODevice::Text);

				//�Ƿ�����ļ�����д���ļ�ͷ
				{
					//WriteHeader(curFilename, currentOutFilename);
				}
			}

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

			//���¼������ֵ ������Щ����
			m_fileMaps[currentOutFilename]->m_box->m_minCorner.x() = std::min(m_fileMaps[currentOutFilename]->m_box->m_minCorner.x(), currentPoint.x());
			m_fileMaps[currentOutFilename]->m_box->m_minCorner.y() = std::min(m_fileMaps[currentOutFilename]->m_box->m_minCorner.y(), currentPoint.y());
			m_fileMaps[currentOutFilename]->m_box->m_minCorner.z() = std::min(m_fileMaps[currentOutFilename]->m_box->m_minCorner.z(), currentPoint.z());
		    
			m_fileMaps[currentOutFilename]->m_box->m_maxCorner.x() = std::max(m_fileMaps[currentOutFilename]->m_box->m_maxCorner.x(), currentPoint.x());
			m_fileMaps[currentOutFilename]->m_box->m_maxCorner.y() = std::max(m_fileMaps[currentOutFilename]->m_box->m_maxCorner.y(), currentPoint.y());
			m_fileMaps[currentOutFilename]->m_box->m_maxCorner.z() = std::max(m_fileMaps[currentOutFilename]->m_box->m_maxCorner.z(), currentPoint.z());
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
		//ȡ���ļ���
		QString fileName = it->first;
		QFileInfo cInfo(fileName);
		QString abpath = cInfo.absolutePath();
		QString basename = cInfo.baseName();
		QString cOutname = abpath + "\\" + basename + "_box" + ".txt";

		QFile boxFile(cOutname);
		if (!boxFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			return eError;
		}
		

		//д��box�ļ�
		QString boxInfo;
		boxInfo.append(QString("%1").arg(it->second->m_box->m_minCorner.x(), 0, 'f', 3));
		boxInfo.append(","); //��ӷָ���
		boxInfo.append(QString("%1").arg(it->second->m_box->m_minCorner.y(), 0, 'f', 3));
		boxInfo.append(","); //��ӷָ���
		boxInfo.append(QString("%1").arg(it->second->m_box->m_minCorner.z(), 0, 'f', 3));

		boxInfo.append(","); //��ӷָ���

		boxInfo.append(QString("%1").arg(it->second->m_box->m_maxCorner.x(), 0, 'f', 3));
		boxInfo.append(","); //��ӷָ���
		boxInfo.append(QString("%1").arg(it->second->m_box->m_maxCorner.y(), 0, 'f', 3));
		boxInfo.append(","); //��ӷָ���
		boxInfo.append(QString("%1").arg(it->second->m_box->m_maxCorner.z(), 0, 'f', 3));

		boxFile.write(boxInfo.toStdString().c_str());
		boxFile.write("\n");

		it->second->m_qdatafile->close();
		delete it->second->m_qdatafile;
		it->second->m_qdatafile = nullptr;
		delete it->second->m_box;
		it->second->m_box = nullptr;

		boxFile.close();
	}
	//foreach(FileTrunkInfo* fi, m_fileMaps)
	//{
	//	//д��box�ļ�
	//	QString boxInfo;
	//	boxInfo.append(QString("%1").arg(fi->m_box->m_minCorner.x(), 0, 'f', 3));
	//	boxInfo.append(","); //��ӷָ���
	//	boxInfo.append(QString("%1").arg(fi->m_box->m_minCorner.y(), 0, 'f', 3));
	//	boxInfo.append(","); //��ӷָ���
	//	boxInfo.append(QString("%1").arg(fi->m_box->m_minCorner.z(), 0, 'f', 3));

	//	boxInfo.append(","); //��ӷָ���

	//	boxInfo.append(QString("%1").arg(fi->m_box->m_maxCorner.x(), 0, 'f', 3));
	//	boxInfo.append(","); //��ӷָ���
	//	boxInfo.append(QString("%1").arg(fi->m_box->m_maxCorner.y(), 0, 'f', 3));
	//	boxInfo.append(","); //��ӷָ���
	//	boxInfo.append(QString("%1").arg(fi->m_box->m_maxCorner.z(), 0, 'f', 3));

	//	fi->m_qBoxfile->write(boxInfo.toStdString().c_str());
	//	fi->m_qBoxfile->write("\n");

	//	fi->m_qdatafile->close();
	//	fi->m_qBoxfile->close();
	//}
	m_fileMaps.clear();

	std::cout << "finish bootom file" << std::endl;
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
	std::cout << current_date.toStdString() << std::endl;
	return eNoError;
}

void AsciiSpliter::WriteBoxTofile(QString infile, unsigned level, const QDir& outDir)
{
	QFileInfo info(infile);

	QString outboxName = QString("%1_%2.%3").arg(info.baseName()).arg(BOXEXT).arg(GetSuffix());
	QString outboxFullname = outDir.filePath(outboxName);
	QFile boxFile(outboxFullname);
	QTextStream boxStream(&boxFile);
	if (!boxFile.open(QIODevice::WriteOnly))
	{
		return;
	}
	boxStream << level << endl;
	boxStream << m_boundBox.m_minCorner.x() << "," << m_boundBox.m_minCorner.y() << "," << m_boundBox.m_minCorner.z() << endl;
	boxStream << m_boundBox.m_maxCorner.x() << "," << m_boundBox.m_maxCorner.y() << "," << m_boundBox.m_maxCorner.z() << endl;
	boxFile.close();
}

void AsciiSpliter::WriteFileByCombine(QString infile, unsigned level, const QDir& outDir, unsigned simple/* = 3*/)
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

			//ͬ�ϴ���box����
			QString cBoxName = QString("%1_box.%2").arg(it->second).arg(GetSuffix());
			QString cBoxurrentOutFilename = outDir.filePath(cBoxName);
			QFile outBoxFile(cBoxurrentOutFilename);
			if (!outBoxFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
			{
				continue;
			}

			//д�ļ�ͷ
			{
				//WriteHeader(cName, currentOutFilename);
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

				//�ļ�ͷ��10�У���Ҫ��������Ȼ���������
				/*for (unsigned i = 0; i < 10; ++i)
				{
					inFile.readLine(currentLine, 500);
				}*/

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
			//д��box
			QString boxInfo;
			boxInfo.append(QString("%1").arg(comBineBox.m_minCorner.x(), 0, 'f', 3));
			boxInfo.append(","); //��ӷָ���
			boxInfo.append(QString("%1").arg(comBineBox.m_minCorner.y(), 0, 'f', 3));
			boxInfo.append(","); //��ӷָ���
			boxInfo.append(QString("%1").arg(comBineBox.m_minCorner.z(), 0, 'f', 3));

			boxInfo.append(","); //��ӷָ���

			boxInfo.append(QString("%1").arg(comBineBox.m_maxCorner.x(), 0, 'f', 3));
			boxInfo.append(","); //��ӷָ���
			boxInfo.append(QString("%1").arg(comBineBox.m_maxCorner.y(), 0, 'f', 3));
			boxInfo.append(","); //��ӷָ���
			boxInfo.append(QString("%1").arg(comBineBox.m_maxCorner.z(), 0, 'f', 3));

			outBoxFile.write(boxInfo.toStdString().c_str());
			outBoxFile.write("\n");

			//�رվ��
			outBoxFile.close();
			outFile.close();
		}
	}

	//std::cout << "finish combine file" << std::endl;
	//QDateTime current_date_time = QDateTime::currentDateTime();
	//QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
	//std::cout << current_date.toStdString() << std::endl;
}

QStringList AsciiSpliter::GenIndexsByIndex(int level, QString index)
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

BoundBox AsciiSpliter::GenSubBoxByFileName(const QString& name)
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

void AsciiSpliter::WriteHeader(QString& curFilename, QString& currentOutFilename)
{
	//����box�Լ���ǰ�ļ�����������д���ļ�ͷ
	BoundBox subbox = GenSubBoxByFileName(currentOutFilename);
	//д�ļ�ͷ
	m_fileMaps[currentOutFilename]->m_qdatafile->write("DCLW PFA");
	m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");

	//! center
	QString centerInfo = QString("Center %1 %2 %3").arg(subbox.GetCenter().x()).arg(subbox.GetCenter().y()).arg(subbox.GetCenter().z());
	m_fileMaps[currentOutFilename]->m_qdatafile->write(centerInfo.toStdString().c_str());
	m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");

	//! Rabgelist
	QString rangeInfo1 = QString("RangeList 2 {");
	m_fileMaps[currentOutFilename]->m_qdatafile->write(rangeInfo1.toStdString().c_str());
	m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");

	QString rangeInfo2 = QString("%1 %2").arg(subbox.m_sphereRadius * 7.0).arg(10000000000);
	m_fileMaps[currentOutFilename]->m_qdatafile->write(rangeInfo2.toStdString().c_str());
	m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");

	QString rangeInfo3 = QString("%1 %2").arg(0).arg(subbox.m_sphereRadius * 7.0);
	m_fileMaps[currentOutFilename]->m_qdatafile->write(rangeInfo3.toStdString().c_str());
	m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");

	QString rangeInfo4 = QString("}");
	m_fileMaps[currentOutFilename]->m_qdatafile->write(rangeInfo4.toStdString().c_str());
	m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");

	//! filenamelist
	QString filenameInfo1 = QString("FileNameList 2 {");
	m_fileMaps[currentOutFilename]->m_qdatafile->write(filenameInfo1.toStdString().c_str());
	m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");

	QString filenameInfo2 = QString("\"\"");
	m_fileMaps[currentOutFilename]->m_qdatafile->write(filenameInfo2.toStdString().c_str());
	m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");

	QString filenameInfo3 = curFilename;
	m_fileMaps[currentOutFilename]->m_qdatafile->write(filenameInfo3.toStdString().c_str());
	m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");

	QString filenameInfo4 = QString("}");
	m_fileMaps[currentOutFilename]->m_qdatafile->write(filenameInfo4.toStdString().c_str());
	m_fileMaps[currentOutFilename]->m_qdatafile->write("\n");
}