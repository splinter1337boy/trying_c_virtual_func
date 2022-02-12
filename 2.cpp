// Example program
#include <iostream>
#include <string>
#include <cassert>

class ArrayInt
{
private:
    int m_length;
    int* m_data;
    
public:
    ArrayInt()
        : m_length(0), m_data(nullptr)    
    {
    }
    
    ArrayInt(int length)
        : m_length(length)
    {
        assert(length >= 0);
        if(length > 0)
            m_data = new int[length];
        else
            m_data = nullptr;
    }
    
    ArrayInt(const std::initializer_list<int>& list) :
        ArrayInt(list.size())
    {
        int count = 0;
        
        for(auto& element : list)
        {
            m_data[count] = element;
            ++count;
        }
    }

	ArrayInt& operator=(const std::initializer_list<int> &list)
	{
		// Если новый список имеет другой размер, то перевыделяем его
		if (list.size() != static_cast<size_t>(m_length))
		{
			// Удаляем все существующие элементы
			delete[] m_data;
 
			// Перевыделяем массив
			m_length = list.size();
			m_data = new int[m_length];
		}
 
		// Теперь инициализируем наш массив значениями из списка
		int count = 0;
		for (auto &element : list)
		{
			m_data[count] = element;
			++count;
		}
 
		return *this;
	}
    
    ~ArrayInt()
    {
        delete[] m_data;    
    }
    
    void erase()
    {
        delete[] m_data;
        m_data = nullptr;
        m_length = 0;
    }
    
    int& operator[] (int index) const
    {
        assert(index >= 0 && index < m_length);
        return m_data[index];
    }
    
    void reallocate(int newLength)
    {
        erase();
        if(newLength <= 0)
            return;
        
        m_data = new int[newLength];
        m_length = newLength;
    }
    
    void resize(int newLength)
    {
        if(newLength == m_length)
            return;
        
        if(newLength <= 0)
        {
            erase();
            return;
        }
        
        int* data = new int[newLength];
        
        if(m_length > 0)
        {
            int elementsToCopy = (newLength > m_length) ? m_length : newLength;
            
            for(int index = 0; index < elementsToCopy; index++)
                data[index] = m_data[index];
        }
        
        delete[] m_data;
        
        m_data = data;
        m_length = newLength;
    }
    
    
    void insertBefore(int value, int index)
    {
        assert(index >= 0 && index < m_length);
        
        int* data = new int[m_length + 1];
        
        for(int before = 0; before < index; before++)
        {
            data[before] = m_data[before];        
        }
        
        data[index] = value;
        
        for(int after = index; after < m_length; after++)
            data[after + 1] = m_data[after];

        delete[] m_data;
        m_data = data;
        m_length++;
    }
    
    void remove(int index)
    {
        assert(index >= 0 && index < m_length);
        
        if(m_length == 1)
        {
            erase();
            return;
        }
        
        
        int* data = new int[m_length - 1];
        
        for(int before = 0; before < index; before++)
        {
            data[before] = m_data[before];
        }
        
        for(int after = index + 1; after < m_length; after++)
        {
            data[after - 1] = m_data[after];
        }
        
        delete[] m_data;
        m_data = data;
        --m_length;
    }
    
    void insertAtBeginning(int value) { insertBefore(value, 0); }
    void insertAtEnd(int value) { insertBefore(value, m_length); }
    
    int getLength() const { return m_length; }
    
    friend std::ostream& operator << (std::ostream& out, const ArrayInt& array)
    {
        out << "\n";
        for(int i = 0; i < array.getLength(); i++)
            out << array[i] << " ";
        
        out << "\n";
        return out;
    }
};


int main()
{
    ArrayInt array { 7, 6, 5, 4, 3, 2, 1 }; // список инициализации
    
    array.resize(7);
    
    array.insertBefore(15, 4);
    
    array.remove(5);
    
    array.insertAtEnd(35);
    array.insertAtBeginning(50);
    
    std::cout << array << std::endl;
    
}
