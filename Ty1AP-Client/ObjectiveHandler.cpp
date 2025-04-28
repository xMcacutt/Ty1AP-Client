#include "ObjectiveHandler.h";

void ObjectiveHandler::HandleObjective(std::string name, int state, std::vector<std::byte>& data) {
	if (name == ObjectiveHandler::objectives[0].name) {
        for (int i = 0; i < 8; i++)
        {
            //if (data[i] != 1 || CurrentData[i] == 1) continue;
            //CurrentData[i] = OldData[i] = data[i];
            //ProcessHandler.TryRead(ObjectAddress + i * 0x19C + 0xA8, out int seahorseActivity, false, "CableCar : IsActive()");
            //if (seahorseActivity == 0x0)
            //    return;
            //ProcessHandler.WriteData(ObjectAddress + i * 0x19C + 0xA8, new byte[]{ 0x0 });
            //CurrentCount = CurrentData.Count(x = > x == ObjectActiveState);
            //ProcessHandler.WriteData((int)TyProcess.BaseAddress + 0x260228 + 0x70, BitConverter.GetBytes(CurrentCount));
        }
	}

	if (name == ObjectiveHandler::objectives[1].name) {

	}
}
