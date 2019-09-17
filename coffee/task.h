#ifndef __task_h_
#define __task_h_
namespace idb {
class Task {
    public:
    virtual void Setup();
    virtual void Run() = 0;
    virtual void Debug() const;
};
}
#endif