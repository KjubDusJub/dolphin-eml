//
// Created by jonas on 3/12/24.
//

#ifndef EXTRACTCOMMAND_H
#define EXTRACTCOMMAND_H

#include <future>
#include <iostream>
#include "DiscIO/DiscExtractor.h"
#include "DiscIO/DiscUtils.h"
#include "DiscIO/Filesystem.h"
#include "DiscIO/Volume.h"
#include <fmt/format.h>
#include <fmt/ostream.h>



namespace DolphinTool
{
    int Extract(const std::vector<std::string>& args);
    void ExtractPartition(const DiscIO::Partition& partition, const std::string& out);
    bool ExtractSystemData(const DiscIO::Partition& partition, const std::string& out);
    DiscIO::Partition GetPartitionFromID(int id);
    void ExtractDirectory(const DiscIO::Partition& partition, const std::string& path,
    const std::string& out);
    void ExtractFile(const DiscIO::Partition& partition, const std::string& path,
    const std::string& out);
}


#endif //EXTRACTCOMMAND_H
