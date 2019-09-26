#ifndef GEOMETRY_H
#define GEOMETRY_H

namespace DC
{
	namespace Pyra
	{
		class Quadnode;
		class Geometry
		{
		public:
			Geometry(Quadnode* node);
			~Geometry();

			virtual void Draw() = 0;

			//��ȡ�����Ľڵ�
			Quadnode* GetNode() const;
		private:
			//�����Ľڵ������Ҫ�Ǵ��л�ȡ����
			Quadnode* m_assNode;
		};


	}
}

#endif // GEOMETRY_H
