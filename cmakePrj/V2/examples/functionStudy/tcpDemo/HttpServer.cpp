#include"TcpServer.h"

using namespace TcpConnect;

int main(int argc, char *argv[])
{
  TcpServer* m_TcpServer= new TcpServer();
  m_TcpServer->start();
  
  delete m_TcpServer;
  return 0;
}