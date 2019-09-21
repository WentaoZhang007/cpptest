#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
using namespace std;
/*
���ӻ�������Ľ����ߣ�
Ҳֻ�������ڲ���һ��ָ��Ͽ���ִ�и���������������������ִ����
�������Open�򿪵���, Close�رյ��ӣ�Change����Ƶ���Ľӿ�
ң����������ķ����ߣ����ں�����Ķ���
�����������ͳ�ȥ�������Ҫ����������Ľӿڣ��൱���˰���ң�����İ�ť
�ں�RunCommand��������Ľӿڣ�������������ӿڵ�Executeִ�к���

ң�����ϵ���Щ��ť��
��ͬ�İ�ť��Ӧ���ӻ��Ĳ�ͬ������
���������ɫ��һ������ӿ������ݣ�
�����������������ʵ���˳�������ӿڣ�
����������������ֱ�������ֲ�����
�򿪵��ӻ����رյ��ӻ����л�Ƶ����

��Ȼ�����ӻ�ң��������һ�����͵�����ģʽӦ��ʵ����
*/

// �����߶��� ���Ӳ������������ִ����
class Television
{
public:
    void Open()       // �򿪵���
    {
        std::cout << "�򿪵���" << std::endl;
    }    

    void Close()      // �رյ���
    {
        std::cout << "�رյ���" << std::endl;
    }
    
    void Change()     // �л�Ƶ��
    {
        std::cout << "����Ƶ��" << std::endl;
    }
};

class Command
{
public:
    virtual void Execute() = 0;         // ִ�еĽӿ�
};

// �򿪵��ӵ�����
class OpenCommand : public Command
{
public:
    OpenCommand(Television *tv)
    {
        m_tv = tv;
    }    

    void Execute()         // ִ�еĽӿ�
    {
        //std::cout <<"�򿪵���" <<std::endl;
        m_tv->Open();
    }

protected:
    Television * m_tv;
};

// �رյ��ӵ�����
class CloseCommand : public Command
{
public:
    CloseCommand(Television *tv)
    {
        m_tv = tv;
    }

    void Execute()         // ִ�еĽӿ�
    {
        //std::cout <<"�رյ���" <<std::endl;
        m_tv->Close();
    }

protected:
    Television * m_tv;
};

// ����Ƶ��������
class ChangeCommand : public Command
{
public:
    ChangeCommand(Television *tv)
    {
        m_tv = tv;
    }

    void Execute()         // ִ�еĽӿ�
    {
        //std::cout <<"����Ƶ��" <<std::endl;
        m_tv->Change();
    }

protected:
    Television * m_tv;
};


// Invoker��������
// Ҫ���������ִ������
// ͨ�������������󣬿��Գ��кܶ���������
class Control
{
public:
    void SetCommand(Command *command) {
        this->m_command = command;
    }
    
    void RunCommand()  {
        m_command->Execute();
    }

protected:
    Command * m_command;
};


// ��������ʵ���൱��������
int main()
{
    // ����������һ��ң����
    Control *control = new Control;
    // ������һ����ʵ�
    Television *tv = new Television;
    /// �������Ǵ򿪵��ӻ�
    // ����һ���򿪵�����, �������ֻ����tv�������
    Command *openCommand = new OpenCommand(tv);
    control->SetCommand(openCommand);
    control->RunCommand();
    delete openCommand;


    // ���˺þ�Ƶ��Ҳû�ҵ��ÿ���, ���ƵĹص����ӻ�
    // ����һ������Ƶ��������������ֻ����tv�������
    Command *changeCommand = new ChangeCommand(tv);
    control->SetCommand(changeCommand);
    control->RunCommand();
    delete changeCommand;
    

    // ���˺þ�Ƶ��Ҳû�ҵ��ÿ���, ���ƵĹص����ӻ�
    // ����һ���رյ�����, �������ֻ����tv�������

    Command *closeCommand = new CloseCommand(tv);
    control->SetCommand(closeCommand);
    control->RunCommand();
    delete closeCommand;

    delete tv;
    delete control;
    return 0;
}