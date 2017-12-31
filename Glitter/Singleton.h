#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
public:
	static T& getInstance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() {};
	~Singleton() {};

private:
	Singleton(Singleton const &) = delete;
	Singleton& operator=(Singleton const &) = delete;
};

#define SINGLETON(class_name) \
	class class_name : public Singleton<class_name> { \
		friend class Singleton<class_name>;

#define SINGLETON_BEGIN(class_name) \
	class class_name : public Singleton<class_name> { \
		friend class Singleton<class_name>;

#define SINGLETON_END() };

#endif // SINGLETON_H