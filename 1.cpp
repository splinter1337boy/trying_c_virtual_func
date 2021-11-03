// Example program
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <initializer_list>

class Person
{
protected:
    std::string m_name;
    int m_age;
    
public:
    Person(const std::string name = nullptr, int age = 0)
        : m_name(name), m_age(age)
    {
    }
    
    const std::string getName() const { return m_name; }
    const int getAge() const  { return m_age; }
    
    virtual std::ostream& print(std::ostream& out) const = 0;
    
    friend std::ostream& operator << (std::ostream& out, const Person& p)
    {
        return p.print(out);
    }
    
    virtual ~Person()
    {
    }
};


class Employee : public Person
{
private:
    int m_salary;
public:
    Employee(const std::string name = nullptr, int age = 0, int salary = 0)
        : Person(name, age),
            m_salary(salary)
    {
    }
    
    const int getSalary() const { return m_salary; }
    
    virtual std::ostream& print(std::ostream& out) const override
    {
        out << "Employee's name is " << m_name << "\nEmployee's age is " << m_age << "\nEmployee's salary is " << m_salary << "\n";
        
        return out;
    }
    
    virtual ~Employee()
    {
    }
};


class Department
{
private:
    std::string m_name;
    Person** m_persons;
    int m_length = 0;
    int m_capacity = 0;
    
public: 
    Department(const std::string name = nullptr, int length = 10)
        : m_name(name), m_length(length)
    {
        assert(length >= 0);
        if(length > 0)
            m_persons = new Person*[length];
        else
            m_persons = nullptr;
    }
    
    Department(const std::string name, const std::initializer_list<Person*> &list)
        : Department(name)
    {
        for(Person* element : list)
        {
            if(m_capacity > 10)
                return;
            m_persons[m_capacity] = element;
            ++m_capacity;
        }
    }
    
    Department& operator = (const std::initializer_list<Person*>& list)
    {
        if(list.size() != static_cast<size_t>(m_length))
        {
            for(int i = 0; i < 10; i++)
                delete m_persons[i];
            delete[] m_persons;
            
            // m_length = list.size();
            m_length = 10;
            m_persons = new Person*[m_length];
        }
        
        for(Person* element : list)
        {
            if(m_capacity > 10)
                return *this;
            m_persons[m_capacity] = element;
            ++m_capacity;
        }
        
        return *this;
    }
    
    Person* getPerson(int i)
    {
        return m_persons[i];    
    }
    
    Person** getPersons()
    {
        return m_persons;        
    }
    
    const std::string getName() const { return m_name; }
    
    int& getCapacity() { return m_capacity; }
            
    friend std::ostream& operator << (std::ostream& out, Department& d);
            
    ~Department()
    {
        for(int i = 0; i < m_length; i++)
            delete m_persons[i];
        delete[] m_persons;
    }
    
    
    void erase()
    {
        for(int i = 0; i < m_length; i++)
            delete m_persons[i];
        delete[] m_persons;
        
        m_persons = nullptr;
        m_length = 0;
    }
    
};


std::ostream& operator << (std::ostream& out, Department& d)
{
    out << "Department: " << d.getName() << "\n";
    
    for(int i = 0; i < d.getCapacity(); i++)
    {
        d.m_persons[i]->print(out);
    }
    
    return out;
}



class Handler
{
private:
    int m_maxSalary;
    
public:
    Handler()
    {
    }
    
    const int getMaxSalary() const { return m_maxSalary; }
    
    Handler* addPerson(Department* dp, Person* p)
    {
        if(dp->getCapacity() > 10)
            return this;
            
        dp->getPersons()[dp->getCapacity()] = p;
        
        dp->getCapacity()++;
        
        return this;
    }
    
    void findMaxSalary(Department* dp)
    {
        int maxSalary = -1;
        for(int i = 0; i < dp->getCapacity(); i++)
        {
            if(Employee* employee = dynamic_cast<Employee*>(dp->getPersons()[i]))
            {
                if(employee->getSalary() > maxSalary)
                    maxSalary = employee->getSalary();
            }
        }
        
        if(maxSalary != -1)
            m_maxSalary = maxSalary;    
    }
    
    ~Handler()
    {  
    }
};


int main()
{
    Handler* hndlr = new Handler();

    
    Employee* e1 = new Employee("Sanya", 30, 30000);
    Employee* e2 = new Employee("Sergei", 40, 50000);
    Employee* e3 = new Employee("Ivan", 18);
    Employee* e4 = new Employee("Sasha", 20);
    Employee* e5 = new Employee("Vanya", 22);
    
    Department* d1 = new Department("Matrix");
    Department* d2 = new Department("Makeevka Online");
    Department* d3 = new Department("SkyNet");
    
    Department* d4 = new Department("Avocado", {new Employee("Pasha", 30, 30000), new Employee("Ilya", 25, 40000)});
    
    hndlr->addPerson(d1, e3)->addPerson(d1, e4);
    hndlr->addPerson(d2, e5)->addPerson(d2, e4);
    hndlr->addPerson(d3, e1)->addPerson(d3, e2);
    hndlr->findMaxSalary(d3);
    
    Person* temp = d1->getPerson(0);
    std::cout << "Name is " << temp->getName() << "\n";
    std::cout << "Age is " << temp->getAge() << "\n";
    
    
    std::cout << *d4 << std::endl;
    std::cout << *d1 << std::endl;
    std::cout << *d2 << std::endl;
    std::cout << *d3 << std::endl;
   
    
    std::cout << "Max Salary is " << hndlr->getMaxSalary() << "\n";
    
    delete temp;
    
    delete d1;
    delete d2;
    delete d3;
    delete d4;
    
    delete e1;
    delete e2;
    delete e3;
    delete e4;
    delete e5;
    
    delete hndlr;
}
