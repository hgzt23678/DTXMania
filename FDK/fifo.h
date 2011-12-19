#pragma once

namespace FDK {
	namespace General {

class FIFO
{
public:
	void 	Clear();						// ������
	void	Push( void *obj );				// �i�[
	void*	Pop();							// ���o��
	void*	Peek() {return m_pFirst;}		// ���Ɏ��o���v�f��`����
	
public:
	FIFO() {m_pFirst=m_pLast=NULL;}

protected:
	struct Cell {
		void*	obj;
		Cell	*prev, *next;
	};
	Cell*	m_pFirst;
	Cell*	m_pLast;
};

	}//General
}//FDK

using namespace FDK::General;
