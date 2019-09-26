#ifndef DC_PYRA_FILESPLITSTRATEGY_H
#define DC_PYRA_FILESPLITSTRATEGY_H


//Qt
#include "QString"
#include "QDir"

#include "Common.h"

namespace DC
{
	namespace Pyra
	{
		class FileSplitStrategy
		{
		public:
			//

			//�ָ�����С�ļ�
			virtual ErrorType Spit(const QString& inFile, const LevelType& level, const QDir& outDir, bool isAll = true) = 0;

		private:
		};
	}
} 

#endif