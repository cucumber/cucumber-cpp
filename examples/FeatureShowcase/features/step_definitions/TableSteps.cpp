#include <gtest/gtest.h>
#include <cucumber-cpp/autodetect.hpp>

#include <string>
#include <map>

using cucumber::ScenarioScope;

class ActiveActors {
public:
  typedef std::string actor_name_type;
  typedef unsigned short actor_year_type;
private:
  typedef std::map<actor_name_type, actor_year_type> actors_type;

  actors_type actors;
public:
  void addActor(const actor_name_type name, const actor_year_type year) {
    actors[name] = year;
  }

  void retireActor(const actor_name_type name) {
    actors.erase(actors.find(name));
  }

  void killActor(const actor_name_type name) {
    actors.erase(actors.find(name));
  }

  actor_name_type getOldestActor() {
    actors_type::iterator it = actors.begin();
    actor_name_type name = it->first;
    actor_year_type year = it->second;
    actor_year_type currentYear = 0;
    while(it != actors.end()) {
      currentYear = it->second;
      if (year > currentYear) {
        name = it->first;
        year = it->second;
      }
      it++;
    }
    return name;
  }

  actor_name_type getYoungestActor() {
    actors_type::iterator it = actors.begin();
    actor_name_type name = it->first;
    actor_year_type bestYearOfBirth = it->second;
    actor_year_type currentYearOfBirth = 0;
    while(it != actors.end()) {
      currentYearOfBirth = it->second;
      if (bestYearOfBirth < currentYearOfBirth) {
        name = it->first;
        bestYearOfBirth = currentYearOfBirth;
      }
      it++;
    }
    return name;
  }

  actor_name_type getSexiestActor() {
    actors_type::iterator it = actors.begin();
    while(it != actors.end()) {
      if (it->first == "George Clooney") {
        return it->first;
      }
      it++;
    }
    return "nobody";
  }
};

GIVEN("^the following actors are still active") {
  TABLE_PARAM(actorsParam);
  ScenarioScope<ActiveActors> context;

  const table_hashes_type & actors = actorsParam.hashes();
  for (table_hashes_type::const_iterator ait = actors.begin(); ait != actors.end(); ++ait) {
    std::string name(ait->at("name"));
    std::string yearString(ait->at("born"));
    const ActiveActors::actor_year_type year = ::cucumber::internal::fromString<ActiveActors::actor_year_type>(yearString);
    context->addActor(name, year);
  }
}

WHEN("^(.+) retires") {
  REGEX_PARAM(std::string, retiringActor);
  ScenarioScope<ActiveActors> context;

  context->retireActor(retiringActor);
}

WHEN("^(.+) dies") {
  REGEX_PARAM(std::string, retiringActor);
  ScenarioScope<ActiveActors> context;

  context->killActor(retiringActor);
}

THEN("^the oldest actor should be (.+)$") {
  REGEX_PARAM(std::string, oldestActor);
  ScenarioScope<ActiveActors> context;

  ASSERT_EQ(oldestActor, context->getOldestActor());
}


THEN("^the youngest actor should be (.+)$") {
  REGEX_PARAM(std::string, youngestActor);
  ScenarioScope<ActiveActors> context;

  ASSERT_EQ(youngestActor, context->getYoungestActor());
}


THEN("^the sexiest actor should be (.+)$") {
  REGEX_PARAM(std::string, sexiestActor);
  ScenarioScope<ActiveActors> context;

  ASSERT_EQ(sexiestActor, context->getSexiestActor());
}
