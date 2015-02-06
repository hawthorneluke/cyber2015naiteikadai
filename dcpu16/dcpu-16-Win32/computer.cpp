#include "computer.h"

#include "hardware.h"
#include "dcpu-16.h"
#include "monitor.h"
#include "keyboard.h"
#include "clock.h"
 
Computer::Computer() {
	running = true;
}
 
int Computer::OnExecute(char args[]) {
 
    SDL_Event Event;


	long long cycleCount = 0;

	long timeOver = 0;
	
 
	//the loop running everything
    while(running) {

		StartCounter();

		if (!step)
		{
			//run hardware
			for (unsigned int i = 0; i <hardware.size(); i++)
			{
				if (hardware[i]->getFrequency() > 0)
				{
					if (cycleCount % (frequency / hardware[i]->getFrequency()) == 0)
					{
						hardware[i]->run();
					}
				}
			}

		}
		else
		{
			keyboard->run();
		}


		//output some info
		if (cycleCount % (int)(cpu->getFrequency() / 1) == 0 && !step)
		{
			printf("\ncycle: %i\n", cpu->getCycle());
			//cpu->memoryDump(0x9000, 32);
			//cpu->memoryDump(0x8000, 8);
			//cpu->registerDump();
		}


		//get time taken to run this loop
		double time = GetCounter();

		__int64 requiredTime = (1000000000 / frequency); //the amount of time it should of taken

		//work out if it took more or less time than required to see how much time we have left over to sleep/used too much time and need to catch up
		timeOver += (long)(time - requiredTime);
		if (timeOver < 0) timeOver = 0;
		else if (timeOver > 1000000000) timeOver = 1000000000; //max limit of a second

		//calc time to sleep (the time it should take to do all cycles minus the actual time taken, minus any over time to let it catch up)
		long sleepTime = (long)(requiredTime - time - timeOver);
		if (sleepTime > 0)
		{
			if (lockFreq) sleep(sleepTime); //sleep			
		}



		cycleCount++;

    }
 
    OnCleanup();
 
    return 0;
}

bool Computer::OnInit(char args[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

	

	//keyboard@
	SDL_EnableKeyRepeat(200 ,SDL_DEFAULT_REPEAT_INTERVAL);
	//SDL_EnableKeyRepeat(200 ,1);
	lockFreq = true;



	fileIsBigEndian = false;
	


	cpu = Hardware::Attach<CPU>(this);

	Hardware::Attach<Monitor>(this);
	keyboard = Hardware::Attach<Keyboard>(this);
	Hardware::Attach<Clock>(this);

	/*
	cpu = new CPU(this);
	hardware.push_back((Hardware*)cpu);
	monitor = new Monitor(this);
	hardware.push_back((Hardware*)monitor);
	*/

	


	
	//if "in" file exists, read it into memory
	ifstream inFile(args, ios::binary);
	if (inFile.is_open())
	{

		int i =0;

		while (!inFile.eof())
		{
			unsigned short data;
			inFile.read((char *)(&data), sizeof(data));

			if (!inFile.fail())
			cpu->memoryWrite(i, toLittleEndian(data));

			i++;
		}

		inFile.close();

	}
	else {
		printf("�t�@�C�� %s ���J���܂���ł���\n", args);
		inFile.close();
		return false;
	}


	return true;
}
  
void Computer::OnCleanup()
{
	for (int i = hardware.size() - 1; i >= 0; i--)
	{
		delete hardware[i];
		hardware.pop_back();
	}

	/*
	SDL_FreeSurface(surfFontCol);
	SDL_FreeSurface(surfFont);
	*/

	SDL_Quit();
}

int Computer::getHardwareCount()
{
	int skip = (dontIncludeCPUInHardwareCount ? 1 : 0);
	return (hardware.size() - skip); //don't include cpu
}

void Computer::getHardwareInfo(unsigned short i, unsigned short &A, unsigned short &B, unsigned short &C, unsigned short &X, unsigned short &Y)
{
	if (dontIncludeCPUInHardwareCount) i++; //skip (don't include) cpu

	

	if (i < hardware.size()) //if hardware exists
	{
		unsigned int id;
		unsigned short version;
		unsigned int manufacturer;

		hardware[i]->getInfo(id, version, manufacturer);


		A = id & 0xffff;
		B = (id >> 16) & 0xffff;

		C = version;

		X = manufacturer & 0xffff;
		Y = (manufacturer >> 16) & 0xffff;
	}
	else
	{
		A = 0;
		B = 0;
		C = 0;
		X = 0;
		Y = 0;
	}
}

void Computer::interruptHardware(unsigned short i)
{
	if (dontIncludeCPUInHardwareCount) i++; //skip (don't include) cpu

	if (i < hardware.size()) //if hardware exists
	{
		hardware[i]->interrupt();
	}
}
 
int main(int argc, char* argv[])
{
	if (argc <= 1) {
		printf("���[�h����v���O�����̃p�X���w�肵�Ă�������\n\n");
		printf("�g��������F\n");
		printf("-step\t\t\t\t�ꎞ��~������ԂŃX�^�[�g\n\n");
		printf("-memdumpstart [�������Ԓn]\t�ꎞ��~��ԂŃX�e�b�v�������A�ǂ�����̃������̃f�[�^���o�͂��邩�i[�������Ԓn]��16�i���Łj\n\n");
		printf("-memdumpsize [��������]\t�ꎞ��~��ԂŃX�e�b�v�������A�o�͂��郁�����̃f�[�^�̌��i[��������]��10�i���Łj\n");

		return -1;
	}

    Computer computer;

	char* fname = "";
	bool startStepping = computer.step;
	int memoryDumpStartArea = computer.memoryDumpAreaStart;
	int memoryDumpAreaSize = computer.memoryDumpAreaSize;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i], "-step") == 0) {
				startStepping = true;
			}
			else if (strcmp(argv[i], "-memdumpstart") == 0) {
				if (i == argc - 1) {
					printf("�������Ԓn�̒l���w�肵�Ă�������\n");
					return -1;
				}
				memoryDumpStartArea = strtoul(argv[++i], NULL, 16);
				if (memoryDumpStartArea == 0L) {
					printf("�������o�͂�Ԓn0x0000����J�n����\n");
				}
			}
			else if (strcmp(argv[i], "-memdumpsize") == 0) {
				if (i == argc - 1) {
					printf("���������̒l���w�肵�Ă�������\n");
					return -1;
				}
				memoryDumpAreaSize = strtoul(argv[++i], NULL, 10);
				if (memoryDumpStartArea == 0L) {
					printf("10�i���̒l %s ���킩��܂���\n", argv[i]);
					return -1;
				}
			}
			else {
				printf("���� %s ���킩��܂���\n", argv[i]);
			}
		}
		else {
			fname = argv[i];
		}
	}

	if (strcmp(fname, "") == 0) {
		printf("���[�h����v���O�����̃p�X���w�肵�Ă�������\n");

		return -1;
	}

	computer.memoryDumpAreaStart = memoryDumpStartArea;
	computer.memoryDumpAreaSize = memoryDumpAreaSize;

	computer.step = startStepping;

	if (computer.OnInit(fname) == false) {
		return -1;
	}

	if (computer.step == true) {
		printf("�ꎞ��~�BF1�ŃX�e�b�v���Ď���CPU�T�C�N�������s�BF2�ōĐ��B\n");
		computer.cpu->registerDump();
		computer.cpu->memoryDump(computer.memoryDumpAreaStart, computer.memoryDumpAreaSize);
	}
	
 
	return computer.OnExecute(fname);
}

