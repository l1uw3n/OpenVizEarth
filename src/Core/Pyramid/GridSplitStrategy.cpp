//c++
#include <math.h>
#include <map>
#include <iostream>

//Qt
#include "QFile"
#include "QTextStream"
#include "QStringList"
#include "QFileInfo"
#include "QDateTime"
#include "QDir"

#include "GridSplitStrategy.h"
//#include "Quadtree.h"
#include "SplitFactory.h"

using namespace DC::Pyra;
 

GridSplitStrategy::GridSplitStrategy()
	: FileSplitStrategy()
	, m_extension(SUFFIX)
	, m_boxSample(1)
	, m_levelSample(2)
{

}

GridSplitStrategy::~GridSplitStrategy()
{

}

ErrorType GridSplitStrategy::Spit(const QString& inFile, const LevelType& level, const QDir& outDir, bool isAll/* = true*/)
{
	m_inFile = inFile;
	m_level = level;

	if (m_inFile.isEmpty() || m_level < 1)
	{
		return eError;
	}

	//���·���������򱨴�
	if (!outDir.exists())
	{
		return eError;
	}

	//��ȡ��չ����������չ�������ò�ͬ�ķֿ���
	QFileInfo info(inFile);
	QString exten = info.suffix();

	//����
	SplitFactory factory(exten);
	SPliter* spliter = factory.CreateSpliter();

	ErrorType hasBound = spliter->GetBoundBoxByFile(m_inFile, m_bound, m_boxSample);

	if (hasBound == eNoError)
	{
		//�ֿ�д��
		ErrorType writeSuccess = spliter->WriteSubfile(m_inFile, outDir, level);

		//�ϲ�����ϡ���������㼶����
		if (writeSuccess == eNoError)
		{
			if (isAll)
			{
				spliter->WriteFileByCombine(m_inFile, level, outDir, m_levelSample);
			}
		}

		//д���߽���ļ�
		spliter->WriteBoxTofile(m_inFile, m_level, outDir);
		
		return writeSuccess;
	}
	return hasBound;
}


void GridSplitStrategy::SetBoxSample(unsigned sample)
{
	m_boxSample = sample;
}

void GridSplitStrategy::SetLevelSample(unsigned sample)
{
	m_levelSample = sample;
}