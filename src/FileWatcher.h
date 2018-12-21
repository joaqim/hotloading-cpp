#pragma once
#include <efsw/include/efsw/efsw.hpp>
//#include <efsw/efsw.hpp>
#include <CommonTypes.h>
#include <map>

#include <iostream>
// Inherits from the abstract listener class, and implements the file action handler
class UpdateListener : public efsw::FileWatchListener {
 public:
  UpdateListener();

  void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "" );
private:
  uint8 _moveCount{0};
  std::map<std::string, uint8> _mMoveCount;
};
