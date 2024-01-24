#pragma once
#include <iostream>

template<typename T> 
class Array2D
{
private:
	T *m_p = NULL;
	int m_Cols;
	int m_Rows;
	size_t CalcIndex(int col, int row) const
	{
#ifndef NDEBUG
		if (col < 0)
			std::cout << "Negative column" << std::endl;
		if (col >= m_Cols) 
		{
			std::cout << "Column overflow" << std::endl;
			exit(0);
		}
		if (row < 0)
			std::cout << "Negative row" << std::endl;

		if (row >= m_Rows)
		{
			std::cout << "Row overflow" << std::endl;
			exit(0);
		}
#endif
		size_t index = row * m_Cols + col;
		return index;
	}
public:
	Array2D() {}
	Array2D(int cols, int rows)
	{
		InitArray2D(cols, rows);
	}

	void InitArray2D(int cols, int rows)
	{
		m_Cols = cols;
		m_Rows = rows;
		if (m_p)
			free(m_p);
		m_p = (T*)malloc(cols * rows * sizeof(T));
	}

	void InitArray2D(int cols, int rows, T initVal)
	{
		InitArray2D(cols, rows);

		for (int i = 0; i < cols * rows; i++)
			m_p[i] = initVal;
	}

	void InitArray2D(int cols, int rows, void* pData)
	{
		m_Cols = cols;
		m_Rows = rows;

		if (m_p)
			free(m_p);

		m_p = (T*)pData;
	}

	~Array2D()
	{
		Destroy();
	}

	void Destroy()
	{
		if (m_p)
		{
			free(m_p);
			m_p = NULL;
		}
	}

	T* GetAddr(int col, int row) const
	{
		size_t index = CalcIndex(col, row);
		return &m_p[index];
	}

	T* GetBaseAddr() const
	{
		return m_p;
	}

	int GetCount() const
	{
		return m_Cols * m_Rows;
	}

	int GetSize() const // in bytes
	{
		return m_Cols * m_Rows * sizeof(T);
	}

	T Get(int col, int row) const
	{
		return *GetAddr(col, row);
	}

	const T Get(int index)
	{
#ifndef NDEBUG
		if (index >= m_Cols * m_Rows)
			exit(0);
#endif
		return m_p[index];
	}

	T At(int col, int row)
	{
		size_t index = CalcIndex(col, row);
		return m_p[index];
	}

	void Set(int index, const T& val)
	{
#ifndef NDEBUG
		if (index >= m_Cols * m_Rows)
		{
			std::cout << "Index out of bounds" << std::endl;
		}
#endif
		m_p[index] = val;
	}

	void Set(int col, int row, const T& val)
	{
		int index = CalcIndex(col, row);
		m_p[index] = val;
	}

	void GetMinMax(T& min, T& max) const
	{
		max = min = m_p[0];
		for (int i = 1; i < m_Cols * m_Rows; i++)
		{
			if (m_p[i] < min)
				min = m_p[i];
			if (m_p[i] > max)
				max = m_p[i];
		}
	}

	void Normalize(T minRange, T maxRange)
	{
		T min, max;
		GetMinMax(min, max);
		if (max <= min)
			return;
		T minMaxDelta = max - min; 
		T minMaxRange = maxRange - minRange;
		for (int i = 0; i < m_Cols * m_Rows; i++)
			m_p[i] = ((m_p[i] - min) / minMaxDelta) * minMaxRange + minRange;
	}

	void PrintFloat()
	{
		for (int y = 0; y < m_Rows; y++)
		{
			std::cout << "y: " << y;
			for (int x = 0; x < m_Cols; x++)
			{
				float f = (float)m_p[y * m_Cols + x];
				std::cout << f << " ";
			}
			std::cout << std::endl;
		}
	}
};