#include "TickTockAdapter.hh"
#include "TickTockController.hh"

#include "AdapterConfiguration.hh"
#include "AdapterFactory.hh"
#include "AdapterExecInterface.hh"
#include "Debug.hh"
#include "StateCacheEntry.hh"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::map;
using std::string;
using std::vector;
using std::copy;


///////////////////////////// Conveniences //////////////////////////////////

// A preamble for error messages.
static string error = "Error in TickTockAdapter: ";

// A prettier name for the "unknown" value.
static Value Unknown;

// A localized handle on the adapter, which allows a
// decoupling between the sample system and adapter.
static TickTockAdapter * Adapter;

// An empty argument vector.
static vector<Value> EmptyArgs;


///////////////////////////// State support //////////////////////////////////

// Queries the system for the value of a state and its arguments.
//
static Value fetch (const string& state_name, const vector<Value>& args)
{
  debugMsg("TickTockAdapter:fetch",
           "Fetch called on " << state_name << " with " << args.size() << " args");
  Value retval;

  // NOTE: A more streamlined approach to dispatching on state name
  // would be nice.

  if (state_name == "TickCounter") retval = GetTicks();
  else if (state_name == "TockCounter") retval = GetTocks();
  else if (state_name == "CurrentState") retval = GetCurrentState();
  else {
    cerr << error << "invalid state: " << state_name << endl;
    retval = Unknown;
  }

  debugMsg("TickTockAdapter:fetch", "Fetch returning " << retval);
  return retval;
}


///////////////////////////// Member functions //////////////////////////////////


TickTockAdapter::TickTockAdapter(AdapterExecInterface& execInterface,
                             const pugi::xml_node& configXml) :
    InterfaceAdapter(execInterface, configXml)
{
  debugMsg("TickTockAdapter", " created.");
}

bool TickTockAdapter::initialize()
{
  g_configuration->defaultRegisterAdapter(this);
  Adapter = this;
  debugMsg("TickTockAdapter", " initialized.");
  return true;
}

bool TickTockAdapter::start()
{
  debugMsg("TickTockAdapter", " started.");
  return true;
}

bool TickTockAdapter::stop()
{
  debugMsg("TickTockAdapter", " stopped.");
  return true;
}

bool TickTockAdapter::reset()
{
  debugMsg("TickTockAdapter", " reset.");
  return true;
}

bool TickTockAdapter::shutdown()
{
  debugMsg("TickTockAdapter", " shut down.");
  return true;
}


// Sends a command (as invoked in a Plexil command node) to the system and sends
// the status, and return value if applicable, back to the executive.
//
void TickTockAdapter::executeCommand(Command *cmd)
{
  const string &name = cmd->getName();
  debugMsg("TickTockAdapter", "Received executeCommand for " << name);

  Value retval = Unknown;

  if (name == "Init") {
    Init();
  }
  else if (name == "Tick") {
    Tick();
  }
  else if (name == "Tock") {
    Tock();
  }
  else
    cerr << error << "invalid command: " << name << endl;

  // This sends a command handle back to the executive.
  m_execInterface.handleCommandAck(cmd, COMMAND_SENT_TO_SYSTEM);
  // This sends the command's return value (if expected) to the executive.
  if (retval != Unknown)
    m_execInterface.handleCommandReturn(cmd, retval);
  m_execInterface.notifyOfExternalEvent();
}

void TickTockAdapter::lookupNow(State const &state, StateCacheEntry &entry)
{
  // This is the name of the state as given in the plan's LookupNow
  string const &name = state.name();
  const vector<Value>& args = state.parameters();
  entry.update(fetch(name, args));
}


void TickTockAdapter::subscribe(const State& state)
{
  debugMsg("TickTockAdapter:subscribe", " processing state "
           << state.name());
  m_subscribedStates.insert(state);
}


void TickTockAdapter::unsubscribe (const State& state)
{
  debugMsg("TickTockAdapter:subscribe", " from state "
           << state.name());
  m_subscribedStates.erase(state);
}

// Does nothing.
void TickTockAdapter::setThresholds (const State& state, double hi, double lo)
{
}

void TickTockAdapter::setThresholds (const State& state, int32_t hi, int32_t lo)
{
}


void TickTockAdapter::propagateValueChange (const State& state,
                                          const vector<Value>& vals) const
{
  if (!isStateSubscribed(state))
    return;
  m_execInterface.handleValueChange(state, vals.front());
  m_execInterface.notifyOfExternalEvent();
}


bool TickTockAdapter::isStateSubscribed(const State& state) const
{
  return m_subscribedStates.find(state) != m_subscribedStates.end();
}

// Necessary boilerplate
extern "C" {
  void initTickTockAdapter() {
    REGISTER_ADAPTER(TickTockAdapter, "TickTockAdapter");
  }
}
