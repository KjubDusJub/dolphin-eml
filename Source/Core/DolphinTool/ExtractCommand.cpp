// Copyright 2024 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later
// Written by jonas@kalsvik.no | 12/03/24

#include <DolphinTool/ExtractCommand.h>

namespace DolphinTool
{
std::shared_ptr<DiscIO::Volume> disc_volume;

DiscIO::Partition GetPartitionFromID(int id)
{
  return id == -1 ? DiscIO::PARTITION_NONE : disc_volume->GetPartitions()[id];
}

void ExtractPartition(const DiscIO::Partition& partition, const std::string& out)
{
  ExtractDirectory(partition, "", out + "/files");
  ExtractSystemData(partition, out);
}

bool ExtractSystemData(const DiscIO::Partition& partition, const std::string& out)
{
  return DiscIO::ExportSystemData(*disc_volume, partition, out);
}

void ExtractDirectory(const DiscIO::Partition& partition, const std::string& path,
                      const std::string& out)
{
  const DiscIO::FileSystem* filesystem = disc_volume->GetFileSystem(partition);
  if (!filesystem)
    return;

  std::unique_ptr<DiscIO::FileInfo> info = filesystem->FindFileInfo(path);
  u32 size = info->GetTotalChildren();

  const bool all = path != "";  // todo: nasty, fix

  std::future<void> future = std::async(std::launch::async, [&] {
    int progress = 0;

    DiscIO::ExportDirectory(*disc_volume, partition, *info, true, path, out,
                            [](const std::string& current) { return false; });
  });
}

void ExtractFile(const DiscIO::Partition& partition, const std::string& path,
                 const std::string& out)
{
  const DiscIO::FileSystem* filesystem = disc_volume->GetFileSystem(partition);
  if (!filesystem)
    return;

  bool success =
      DiscIO::ExportFile(*disc_volume, partition, filesystem->FindFileInfo(path).get(), out);
}

int Extract(const std::vector<std::string>& args)
{
  disc_volume = DiscIO::CreateVolume(args[0]);
  auto folder = args[1];
  if (folder == "")
    return 1;
  if (disc_volume->GetPartitions().empty())
  {
    ExtractPartition(DiscIO::PARTITION_NONE, folder);
  }
  else
  {
    for (DiscIO::Partition& p : disc_volume->GetPartitions())
    {
      if (const std::optional<u32> partition_type = disc_volume->GetPartitionType(p))
      {
        const std::string partition_name = DiscIO::NameForPartitionType(*partition_type, true);
        ExtractPartition(p, folder + '/' + partition_name);
      }
    }
  }

  return 0;
}

}
