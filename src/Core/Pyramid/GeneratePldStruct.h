#ifndef GENERATE_PLD_STRUCT_H
#define GENERATE_PLD_STRUCT_H
#include <QString>
#include <QDir>
#include "BoundBox.h"

class GeneratePldStruct
{
public: 
	GeneratePldStruct(DC::Pyra::BoundBox mainBox, QString postFix);
	~GeneratePldStruct();

	void WriteOsgBoxToFile(QString infile, unsigned level, const QDir& outDir);
private:
	//��������ֵ�����ĸ��ӿ������ֵ
	QStringList GenIndexsByIndex(int level, QString index);
	
	//�� �� �� �� �߽� �����Ӧ�ı߽�
	DC::Pyra::BoundBox& CalcBBox(int level, unsigned col, unsigned row, DC::Pyra::BoundBox mainBox);

	osg::ref_ptr<osg::PagedLOD> CreatePagedLOD(DC::Pyra::BoundBox box, QString fileBaseName);
private:
	DC::Pyra::BoundBox m_mainBox;
	QString m_postFix;
};
#endif