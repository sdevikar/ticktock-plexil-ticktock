//
// This is literally the entire code that I am trying to control from the plexil side
// That is, plexil plan will execute this code and call these commands in the order it wishes
//
#include <iostream>

using namespace std;

typedef enum STATES{
  TICK = 0,
  TOCK,
  UNKNOWN
}STATES;

int m_CurrentState;
int TickCounter;
int TockCounter;

// this will support the Init command
void Init()
{
  m_CurrentState = STATES::UNKNOWN;
  TickCounter = 0;
  TockCounter = 0;
}

// this will support the Tick command
void Tick()
{
  m_CurrentState = STATES::TICK;
  TickCounter++;
  cout << "Tick" << endl;
}

// this will support Tock command
void Tock()
{
  m_CurrentState = STATES::TOCK;
  TockCounter++;
  cout << "Tock" << endl;
}

// this will support the state lookup
int GetCurrentState()
{
  return m_CurrentState;
}

int GetTicks()
{
  return TickCounter;
}

int GetTocks()
{
  return TockCounter;
}
