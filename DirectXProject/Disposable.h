#pragma once
#include <vector>


class Disposable
{
protected:
	bool isDisposed = false;
public:
	Disposable();
	~Disposable();
	void Dispose();
	bool IsDisposed();
	virtual void OnDispose() = 0;
	static void DisposeAll(){
		std::set<Disposable*>::iterator it = Disposables->begin();
		for (; it != Disposables->end(); it++)
		{
			(*it)->Dispose();
		}
	};
};

