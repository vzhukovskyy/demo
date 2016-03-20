#include "SharedMutex.h"
#include <thread>
#include <cassert>

void lockReader(SharedMutex &m, bool &blocked, bool &exception)
{
	exception = false;
	blocked = true;  
	try {
		m.shared_lock();
	}
	catch (...) {
		exception = true;
	}
	blocked = false;
}

void unlockReader(SharedMutex &m, bool &blocked, bool &exception)
{
	exception = false;
	blocked = true;
	try {
		m.shared_unlock();
	}
	catch (...) {
		exception = true;
	}
	blocked = false;
}

void lockWriter(SharedMutex &m, bool &blocked, bool &exception)
{
	exception = false;
	blocked = true;
	try {
		m.lock();
	}
	catch (...) {
		exception = true;
	}
	blocked = false;
}

void unlockWriter(SharedMutex &m, bool &blocked, bool &exception)
{
	exception = false;
	blocked = true;
	try {
		m.unlock();
	}
	catch (...) {
		exception = true;
	}
	blocked = false;
}

void unlockIfLocked(SharedMutex &m)
{
	try	{
		m.unlock();
	}
	catch (...) {
	}
}

void sharedUnlockIfSharedLocked(SharedMutex &m)
{
	try {
		m.shared_unlock();
	}
	catch (...) {
	}
}

void test_0readers0writers_unlockWriter(SharedMutex &m)
{
	// 0 readers, 0 writers

	// + unlock reader
	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(unlockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception
	assert(writerThreadBlocked == false);
	assert(writerThreadException == true);

	// release threads
	writer.join();
}

void test_1reader0writers_unlockWriter(SharedMutex &m)
{
	// 1 reader
	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(lockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(readerThreadBlocked == false);
	assert(readerThreadException == false);
	reader.join();

	// + unlock reader
	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(unlockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception
	assert(writerThreadBlocked == false);
	assert(writerThreadException == true);

	// release threads
	writer.join();
}

void test_2readers0writers_unlockWriter(SharedMutex &m)
{
	// 2 readers
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	// + unlock writer
	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(unlockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception
	assert(writerThreadBlocked == false);
	assert(writerThreadException == true);

	// release threads
	writer.join();
}

void test_0readers0writers_unlockReader(SharedMutex &m)
{
	// 0 readers, 0 writers

	// + unlock reader
	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(unlockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception
	assert(readerThreadBlocked == false);
	assert(readerThreadException == true);

	// release threads
	reader.join();
}

void test_0readers0writers_lockReader(SharedMutex &m)
{
	// 0 readers, 0 writers

	// + lock reader
	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(lockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = lock acquired
	assert(readerThreadBlocked == false);
	assert(readerThreadException == false);

	// release threads
	reader.join();
}

void test_1reader0writers_unlockReader(SharedMutex &m)
{
	// 1 reader
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	// + unlock reader
	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(unlockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = lock relinquished
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);

	// release threads
	reader2.join();
}

void test_1reader0writers_lockReader(SharedMutex &m)
{
	// 1 reader
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	// + lock reader
	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = lock acquired
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);

	// release threads
	reader2.join();
}

void test_2readers0writers_unlockReader(SharedMutex &m)
{
	// 2 readers
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	// + unlock reader
	bool reader3ThreadBlocked;
	bool reader3ThreadException;
	std::thread reader3(unlockReader, std::ref(m), std::ref(reader3ThreadBlocked), std::ref(reader3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = lock relinguished
	assert(reader3ThreadBlocked == false);
	assert(reader3ThreadException == false);

	// release threads
	reader3.join();
}


void test_2readers0writers_lockReader(SharedMutex &m)
{
	// 2 readers
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	// + lock reader
	bool reader3ThreadBlocked;
	bool reader3ThreadException;
	std::thread reader3(unlockReader, std::ref(m), std::ref(reader3ThreadBlocked), std::ref(reader3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = lock acquired
	assert(reader3ThreadBlocked == false);
	assert(reader3ThreadException == false);

	// release threads
	reader3.join();
}

void test_0reader0writers_lockWriter(SharedMutex &m)
{
	// 0 readers, 0 writers

	// + lock writer
	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(lockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = write lock acquired
	assert(writerThreadBlocked == false);
	assert(writerThreadException == false);

	// release threads
	writer.join();
}

void test_0reader1writer_unlockWriter(SharedMutex &m)
{
	// 1 writer
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	// + unlock writer
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(unlockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = lock relinguished
	assert(writer2ThreadBlocked == false);
	assert(writer2ThreadException == false);

	// release threads
	writer2.join();
}

void test_0reader1writers_unlockReader(SharedMutex &m)
{
	// 1 writer
	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(lockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writerThreadBlocked == false);
	assert(writerThreadException == false);
	writer.join();

	// + unlock reader
	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(unlockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception
	assert(readerThreadBlocked == false);
	assert(readerThreadException == true);

	// release threads
	reader.join();
}
void test_0reader1writers_lockReader(SharedMutex &m)
{
	// 1 writer
	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(lockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writerThreadBlocked == false);
	assert(writerThreadException == false);
	writer.join();

	// + lock reader
	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(lockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = reader blocked
	assert(readerThreadBlocked == true);
	assert(readerThreadException == false);

	// release threads
	m.unlock();
	reader.join();
}

void test_1reader1writer_readerBlocked_unlockReader(SharedMutex &m)
{
	//  1 writer, 1 reader(blocked)
	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(lockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writerThreadBlocked == false);
	assert(writerThreadException == false);
	writer.join();

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	// + unblock some other reader by mistake
	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(unlockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception and reader still blocked
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == true);
	reader2.join();
	assert(reader1ThreadBlocked == true);

	// release threads
	m.unlock();
	reader1.join();
}

void test_1reader1writer_writerBlocked_unlockReader(SharedMutex &m)
{
	// 1 reader, 1 writer(blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(lockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writerThreadBlocked == true);
	assert(writerThreadException == false);

	// + unlock reader
	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(unlockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = unblocked writer
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();
	assert(writerThreadBlocked == false);

	// release threads
	writer.join();
}

void test_1reader1writer_blockedReader_lockReader(SharedMutex &m)
{
	// 1 writer, 1 reader (blocked)
	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(lockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writerThreadBlocked == false);
	assert(writerThreadException == false);
	writer.join();

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	// + lock reader
	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = 2nd reader blocked, 1st reader remain blocked
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);
	assert(reader1ThreadBlocked == true);

	// release threads
	m.unlock();
	reader2.join();
	reader1.join();
}

void test_1reader1writer_blockedWriter_lockReader(SharedMutex &m)
{
	// 1 reader, 1 writer (blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(lockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writerThreadBlocked == true);
	assert(writerThreadException == false);

	// + lock reader
	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = reader acquires lock, writer still blocked
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();
	assert(writerThreadBlocked == true);

	// release threads
	m.shared_unlock();
	m.shared_unlock();
	writer.join();
}

void test_1reader0writers_lockWriter(SharedMutex &m)
{
	// 1 reader
	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(lockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(readerThreadBlocked == false);
	assert(readerThreadException == false);
	reader.join();

	// + lock writer
	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(lockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = blocked writer
	assert(writerThreadBlocked == true);
	assert(writerThreadException == false);

	// release threads
	m.shared_unlock();
	writer.join();
}

void test_1reader1writer_blockedReader_unlockWriter(SharedMutex &m)
{
	// 1 writer, 1 reader (blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(lockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(readerThreadBlocked == true);
	assert(readerThreadException == false);

	// + unlock writer
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(unlockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = reader unblocked
	assert(writer2ThreadBlocked == false);
	assert(writer2ThreadException == false);
	writer2.join();
	assert(readerThreadBlocked == false);

	// release threads
	reader.join();
}

void test_1reader1writer_blockedWriter_unlockWriter(SharedMutex &m)
{
	// 1 reader, 1 writer (blocked)
	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(lockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(readerThreadBlocked == false);
	assert(readerThreadException == false);
	reader.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	// + unlock some other writer by mistake
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(unlockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception and does not unblock the writer
	assert(writer2ThreadBlocked == false);
	assert(writer2ThreadException == true);
	writer2.join();
	assert(writer1ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	writer1.join(); 
}

void test_2readers0writers_lockWriter(SharedMutex &m)
{
	// 2 readers
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	// + lock writer
	bool writerThreadBlocked;
	bool writerThreadException;
	std::thread writer(lockWriter, std::ref(m), std::ref(writerThreadBlocked), std::ref(writerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = writer blocked
	assert(writerThreadBlocked == true);
	assert(writerThreadException == false);

	// release threads
	m.shared_unlock();
	m.shared_unlock();
	writer.join();
}

void test_2readers1writer_blockedReaders_unlockWriter(SharedMutex &m)
{
	// 1 writer, 2 readers (blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);

	// + unlock writer
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(unlockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = readers unblocked
	assert(writer2ThreadBlocked == false);
	assert(writer2ThreadException == false);
	writer2.join();
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);

	// release threads
	reader1.join();
	reader2.join();
}

void test_2readers1writer_blockedWriter_unlockWriter(SharedMutex &m)
{
	// 2 readers, 1 writer (blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	// + unlock other writer by mistake
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(unlockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception, writer still blocked
	assert(writer2ThreadBlocked == false);
	assert(writer2ThreadException == true);
	writer2.join();
	assert(writer1ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	m.shared_unlock();
	writer1.join();
}

void test_2readers1writer_blockedReaders_unlockReader(SharedMutex &m)
{
	// 1 writer, 2 readers (blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);

	// + unlock another reader by mistake
	bool reader3ThreadBlocked;
	bool reader3ThreadException;
	std::thread reader3(unlockReader, std::ref(m), std::ref(reader3ThreadBlocked), std::ref(reader3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception, readers still blocked
	assert(reader3ThreadBlocked == false);
	assert(reader3ThreadException == true);
	reader3.join();
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);

	// release threads
	m.unlock();
	reader1.join();
	reader2.join();
}

void test_2readers1writer_blockedWriter_unlockReader(SharedMutex &m)
{
	// 2 readers, 1 writer (blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	// + unlock reader
	bool reader3ThreadBlocked;
	bool reader3ThreadException;
	std::thread reader3(unlockReader, std::ref(m), std::ref(reader3ThreadBlocked), std::ref(reader3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = writer still blocked
	assert(reader3ThreadBlocked == false);
	assert(reader3ThreadException == false);
	reader3.join();
	assert(writer1ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	writer1.join();
}


void test_2readers1writer_blockedReaders_lockReader(SharedMutex &m)
{
	// 1 writer, 2 readers (blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);

	// + lock reader
	bool reader3ThreadBlocked;
	bool reader3ThreadException;
	std::thread reader3(lockReader, std::ref(m), std::ref(reader3ThreadBlocked), std::ref(reader3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = 3 readers blocked
	assert(reader3ThreadBlocked == true);
	assert(reader3ThreadException == false);
	assert(reader1ThreadBlocked == true);
	assert(reader2ThreadBlocked == true);

	// release threads
	m.unlock();
	reader1.join();
	reader2.join();
	reader3.join();
}

void test_2readers1writer_blockedWriter_lockReader(SharedMutex &m)
{
	// 2 readers, 1 writer (blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	// + lock reader
	bool reader3ThreadBlocked;
	bool reader3ThreadException;
	std::thread reader3(lockReader, std::ref(m), std::ref(reader3ThreadBlocked), std::ref(reader3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = writer still blocked
	assert(reader3ThreadBlocked == false);
	assert(reader3ThreadException == false);
	reader3.join();
	assert(writer1ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	m.shared_unlock();
	m.shared_unlock();
	writer1.join();
}

void test_0readers1writers_lockWriter(SharedMutex &m)
{
	// 1 writer
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	// + lock writer
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = blocked writer
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// release threads
	m.unlock();
	writer2.join();
}

void test_0readers2writers_unlockWriter(SharedMutex &m)
{
	// 2 writer
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + unlock writer
	bool writer3ThreadBlocked;
	bool writer3ThreadException;
	std::thread writer3(unlockWriter, std::ref(m), std::ref(writer3ThreadBlocked), std::ref(writer3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = unblocked writer
	assert(writer3ThreadBlocked == false);
	assert(writer3ThreadException == false);
	writer3.join();
	assert(writer2ThreadBlocked == false);
	assert(writer2ThreadException == false);

	// release threads
	writer2.join();
}

void test_1reader1writer_blockedReader_lockWriter(SharedMutex &m)
{
	// 1 writer, 1 reader (blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(lockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(readerThreadBlocked == true);
	assert(readerThreadException == false);

	// + lock writer
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = 2nd writer blocked, reader remain blocked
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);
	assert(readerThreadBlocked == true);

	// release threads
	m.unlock(); 
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	// either writer wins the competition for the lock or reader
	sharedUnlockIfSharedLocked(m);
	unlockIfLocked(m);
	reader.join();
	writer2.join();
}

void test_1reader1writer_blockedWriter_lockWriter(SharedMutex &m)
{
	// 1 reader, 1 writer (blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	// + lock another writer
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = 2nd writer blocked, 1st writer remain blocked
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);
	assert(writer1ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	writer1.join();
	writer2.join();
}

void test_1reader2writers_blockedReaderAndWriter_unlockWriter(SharedMutex &m)
{
	// 2 writers (1 blocked), 1 reader (blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	// + unlock writer
	bool writer3ThreadBlocked;
	bool writer3ThreadException;
	std::thread writer3(unlockWriter, std::ref(m), std::ref(writer3ThreadBlocked), std::ref(writer3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = either writer wins the lock, or the reader
	assert(writer3ThreadBlocked == false);
	assert(writer3ThreadException == false);
	writer3.join();
	assert(writer2ThreadBlocked != reader1ThreadBlocked);
	assert(writer2ThreadException == false);
	assert(reader1ThreadException == false);

	// release threads
	sharedUnlockIfSharedLocked(m);
	unlockIfLocked(m);
	writer2.join();
	reader1.join();
}

void test_1reader2writers_blockedWriters_unlockWriter(SharedMutex &m)
{
	// 1 reader, 2 writers (2 blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + unlock another writer by mistake
	bool writer3ThreadBlocked;
	bool writer3ThreadException;
	std::thread writer3(unlockWriter, std::ref(m), std::ref(writer3ThreadBlocked), std::ref(writer3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception, both writers remain blocked
	assert(writer3ThreadBlocked == false);
	assert(writer3ThreadException == true);
	writer3.join();
	assert(writer1ThreadBlocked == true);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	writer1.join();
	writer2.join();
}

void test_2readers1writer_blockedReaders_lockWriter(SharedMutex &m)
{
	// 1 writer, 2 readers (blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);

	// + lock writer
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = blocked writer, readers remain blocked
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);
	assert(reader1ThreadBlocked == true);
	assert(reader2ThreadBlocked == true);

	// release threads
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	sharedUnlockIfSharedLocked(m);
	sharedUnlockIfSharedLocked(m);
	unlockIfLocked(m);
	reader1.join();
	reader2.join();
	writer2.join();

}

void test_2readers1writer_blockedWriter_lockWriter(SharedMutex &m)
{
	// 2 readers, 1 writer (blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	// + lock writer
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = writer blocked, old writer remain blocked
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);
	assert(writer1ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	m.shared_unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	writer1.join();
	writer2.join();
}

void test_2readers2writers_blockedReadersAndWriter_unlockWriter(SharedMutex &m)
{
	// 2 writers (1 blocked),  2 readers (2 blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);

	// + unlock writer
	bool writer3ThreadBlocked;
	bool writer3ThreadException;
	std::thread writer3(unlockWriter, std::ref(m), std::ref(writer3ThreadBlocked), std::ref(writer3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = either one of readers grabs the lock or writer
	assert(writer3ThreadBlocked == false);
	assert(writer3ThreadException == false);
	writer3.join();
	assert((reader1ThreadBlocked == false && reader2ThreadBlocked == false && writer2ThreadBlocked == true) ||
		   (reader1ThreadBlocked == true && reader2ThreadBlocked == true && writer2ThreadBlocked == false));

	// release threads
	if (writer2ThreadBlocked) {
		m.shared_unlock();
		m.shared_unlock();
	}
	else {
		m.unlock();
	}
	reader1.join();
	reader2.join();
	writer2.join();
}

void test_2readers2writers_blockedWriters_unlockWriter(SharedMutex &m)
{
	// 2 readers, 2 writers (2 blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);
	
	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + unlock some other writer by mistake
	bool writer3ThreadBlocked;
	bool writer3ThreadException;
	std::thread writer3(unlockWriter, std::ref(m), std::ref(writer3ThreadBlocked), std::ref(writer3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception, writers remain locked
	assert(writer3ThreadBlocked == false);
	assert(writer3ThreadException == true);
	writer3.join();
	assert(writer1ThreadBlocked == true);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	m.shared_unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	writer1.join(); 
	writer2.join();
}

void test_0readers2writers_unlockReader(SharedMutex &m)
{
	// 2 writers (1 blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + unlock reader
	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(unlockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception, writer remain blocked
	assert(readerThreadBlocked == false);
	assert(readerThreadException == true);
	reader.join();
	assert(writer2ThreadBlocked == true);

	// release threads
	m.unlock();
	writer2.join();
}

void test_0readers2writers_lockReader(SharedMutex &m)
{
	// 2 writers (1 blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + lock reader
	bool readerThreadBlocked;
	bool readerThreadException;
	std::thread reader(lockReader, std::ref(m), std::ref(readerThreadBlocked), std::ref(readerThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = blocked reader
	assert(readerThreadBlocked == true);
	assert(readerThreadException == false);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	unlockIfLocked(m);
	sharedUnlockIfSharedLocked(m);
	writer2.join();
	reader.join();
}

void test_1reader2writers_blockedReaderAndWriter_unlockReader(SharedMutex &m)
{
	// 2 writers (1 blocked), 1 reader (blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	// + unlock reader by mistake
	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(unlockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception, old reader and writer remain blocked
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == true);
	reader2.join();
	assert(writer2ThreadBlocked == true);
	assert(reader1ThreadBlocked == true);

	// release threads
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	unlockIfLocked(m);
	sharedUnlockIfSharedLocked(m);
	writer2.join();
	reader1.join();
}

void test_1reader2writers_blockedWriters_unlockReader(SharedMutex &m)
{
	// 1 reader, 2 writers (2 blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + unlock reader
	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(unlockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = one writer acquired the lock, the other writer remains blocked
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();
	assert(writer1ThreadBlocked != writer2ThreadBlocked);
	assert(writer1ThreadException == false);
	assert(writer2ThreadException == false);

	// release threads
	m.unlock();
	writer1.join();
	writer2.join();
}

void test_1reader2writers_blockedReaderAndWriter_lockReader(SharedMutex &m)
{
	// 2 writers (1 blocked), 1 reader (blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	// + unlock reader by mistake
	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// both readers blocked and writer remain blocked
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);
	assert(writer2ThreadBlocked == true);
	assert(reader1ThreadBlocked == true);

	// release threads
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	unlockIfLocked(m);
	sharedUnlockIfSharedLocked(m);
	sharedUnlockIfSharedLocked(m);
	writer2.join();
	reader1.join();
	reader2.join();

}

void test_1reader2writers_blockedWriters_lockReader(SharedMutex &m)
{
	// 1 reader, 2 writers (2 blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + lock reader
	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = lock acquired, both writers remain blocked
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();
	assert(writer1ThreadBlocked == true);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	m.shared_unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	writer1.join();
	writer2.join();
}

void test_2readers2writers_blockedReadersAndWriter_unlockReader(SharedMutex &m)
{
	// 2 writers (1 blocked),  2 readers (2 blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);

	// + unlock reader by error
	bool reader3ThreadBlocked;
	bool reader3ThreadException;
	std::thread reader3(unlockReader, std::ref(m), std::ref(reader3ThreadBlocked), std::ref(reader3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = exception, old readers and writer remain blocked
	assert(reader3ThreadBlocked == false);
	assert(reader3ThreadException == true);
	reader3.join();
	assert(reader1ThreadBlocked == true);
	assert(reader2ThreadBlocked == true);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	unlockIfLocked(m);
	sharedUnlockIfSharedLocked(m);
	sharedUnlockIfSharedLocked(m);
	reader1.join();
	reader2.join();
	writer2.join();
}

void test_2readers2writers_blockedWriters_unlockReader(SharedMutex &m)
{
	// 2 readers, 2 writers (2 blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + unlock reader
	bool reader3ThreadBlocked;
	bool reader3ThreadException;
	std::thread reader3(unlockReader, std::ref(m), std::ref(reader3ThreadBlocked), std::ref(reader3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// lock still kept by the other reader, writers remain locked
	assert(reader3ThreadBlocked == false);
	assert(reader3ThreadException == false);
	reader3.join();
	assert(writer1ThreadBlocked == true);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	writer1.join();
	writer2.join();
}

void test_2readers2writers_blockedReadersAndWriter_lockReader(SharedMutex &m)
{
	// 2 writers (1 blocked),  2 readers (2 blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);

	// + lock reader
	bool reader3ThreadBlocked;
	bool reader3ThreadException;
	std::thread reader3(lockReader, std::ref(m), std::ref(reader3ThreadBlocked), std::ref(reader3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = readers and writer remain blocked
	assert(reader3ThreadBlocked == true);
	assert(reader3ThreadException == false);
	assert(reader1ThreadBlocked == true);
	assert(reader2ThreadBlocked == true);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	sharedUnlockIfSharedLocked(m);
	sharedUnlockIfSharedLocked(m);
	sharedUnlockIfSharedLocked(m);
	unlockIfLocked(m);
	reader1.join();
	reader2.join();
	reader3.join();
	writer2.join();
}

void test_2readers2writers_blockedWriters_lockReader(SharedMutex &m)
{
	// 2 readers, 2 writers (2 blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + lock reader
	bool reader3ThreadBlocked;
	bool reader3ThreadException;
	std::thread reader3(lockReader, std::ref(m), std::ref(reader3ThreadBlocked), std::ref(reader3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = now 3 readers shares the lock, writers remain locked
	assert(reader3ThreadBlocked == false);
	assert(reader3ThreadException == false);
	reader3.join();
	assert(writer1ThreadBlocked == true);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	m.shared_unlock();
	m.shared_unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	writer1.join();
	writer2.join();
}

void test_0readers2writers_lockWriter(SharedMutex &m)
{
	// 2 writers (1 blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + lock writer
	bool writer3ThreadBlocked;
	bool writer3ThreadException;
	std::thread writer3(lockWriter, std::ref(m), std::ref(writer3ThreadBlocked), std::ref(writer3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = writer blocked, old writer remains blocked
	assert(writer3ThreadBlocked == true);
	assert(writer3ThreadException == false);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	writer2.join();
	writer3.join();
}


void test_1reader2writers_blockedReaderAndWriter_lockWriter(SharedMutex &m)
{
	// 2 writers (1 blocked), 1 reader (blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	// + lock writer
	bool writer3ThreadBlocked;
	bool writer3ThreadException;
	std::thread writer3(lockWriter, std::ref(m), std::ref(writer3ThreadBlocked), std::ref(writer3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// writer blocked, readers and old writer remain blocked
	assert(writer3ThreadBlocked == true);
	assert(writer3ThreadException == false);
	assert(writer2ThreadBlocked == true);
	assert(reader1ThreadBlocked == true);

	// release threads
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	unlockIfLocked(m);
	sharedUnlockIfSharedLocked(m);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	unlockIfLocked(m);
	sharedUnlockIfSharedLocked(m);
	writer2.join();
	writer3.join();
	reader1.join();
}

void test_1reader2writers_blockedWriters_lockWriter(SharedMutex &m)
{
	// 1 reader, 2 writers (2 blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + lock writer
	bool writer3ThreadBlocked;
	bool writer3ThreadException;
	std::thread writer3(lockWriter, std::ref(m), std::ref(writer3ThreadBlocked), std::ref(writer3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = writer blocked, old writers remain blocked
	assert(writer3ThreadBlocked == true);
	assert(writer3ThreadException == false);
	assert(writer1ThreadBlocked == true);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	writer1.join();
	writer2.join();
	writer3.join();

}


void test_2readers2writers_blockedReadersAndWriter_lockWriter(SharedMutex &m)
{
	// 2 writers (1 blocked),  2 readers (2 blocked)
	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == false);
	assert(writer1ThreadException == false);
	writer1.join();

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == true);
	assert(reader1ThreadException == false);

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == true);
	assert(reader2ThreadException == false);

	// + lock writer
	bool writer3ThreadBlocked;
	bool writer3ThreadException;
	std::thread writer3(lockWriter, std::ref(m), std::ref(writer3ThreadBlocked), std::ref(writer3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = writer blocked, readers and old writer remain blocked
	assert(writer3ThreadBlocked == true);
	assert(writer3ThreadException == false);
	assert(reader1ThreadBlocked == true);
	assert(reader2ThreadBlocked == true);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	sharedUnlockIfSharedLocked(m);
	sharedUnlockIfSharedLocked(m);
	unlockIfLocked(m);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	sharedUnlockIfSharedLocked(m);
	sharedUnlockIfSharedLocked(m);
	unlockIfLocked(m);
	reader1.join();
	reader2.join();
	writer3.join();
	writer2.join();
}

void test_2readers2writers_blockedWriters_lockWriter(SharedMutex &m)
{
	// 2 readers, 2 writers (2 blocked)
	bool reader1ThreadBlocked;
	bool reader1ThreadException;
	std::thread reader1(lockReader, std::ref(m), std::ref(reader1ThreadBlocked), std::ref(reader1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader1ThreadBlocked == false);
	assert(reader1ThreadException == false);
	reader1.join();

	bool reader2ThreadBlocked;
	bool reader2ThreadException;
	std::thread reader2(lockReader, std::ref(m), std::ref(reader2ThreadBlocked), std::ref(reader2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(reader2ThreadBlocked == false);
	assert(reader2ThreadException == false);
	reader2.join();

	bool writer1ThreadBlocked;
	bool writer1ThreadException;
	std::thread writer1(lockWriter, std::ref(m), std::ref(writer1ThreadBlocked), std::ref(writer1ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer1ThreadBlocked == true);
	assert(writer1ThreadException == false);

	bool writer2ThreadBlocked;
	bool writer2ThreadException;
	std::thread writer2(lockWriter, std::ref(m), std::ref(writer2ThreadBlocked), std::ref(writer2ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assert(writer2ThreadBlocked == true);
	assert(writer2ThreadException == false);

	// + lock writer
	bool writer3ThreadBlocked;
	bool writer3ThreadException;
	std::thread writer3(lockWriter, std::ref(m), std::ref(writer3ThreadBlocked), std::ref(writer3ThreadException));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// = 2 readers holding the lock, all writers blocked
	assert(writer3ThreadBlocked == true);
	assert(writer3ThreadException == false);
	assert(writer1ThreadBlocked == true);
	assert(writer2ThreadBlocked == true);

	// release threads
	m.shared_unlock();
	m.shared_unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m.unlock();
	writer1.join();
	writer2.join();
	writer3.join();
}

int main()
{
	// writers\readers  0   1   2
	//                  ^   ^   ^
	// 0			    .   .   .
	// 1
	// 2
	//
	test_0readers0writers_unlockWriter(SharedMutex());
	test_1reader0writers_unlockWriter(SharedMutex());
	test_2readers0writers_unlockWriter(SharedMutex());

	// writers\readers  0   1   2
	//
	// 0			  <-.<->.<->.->
	// 1
	// 2
	//
	test_0readers0writers_unlockReader(SharedMutex());
	test_0readers0writers_lockReader(SharedMutex());
	test_1reader0writers_unlockReader(SharedMutex());
	test_1reader0writers_lockReader(SharedMutex());
	test_2readers0writers_unlockReader(SharedMutex());
	test_2readers0writers_lockReader(SharedMutex());

	// writers\readers  0   1   2
	//
	// 0                ^   ^   ^
	// 1                v   v   v
	// 2
	//
	test_0reader0writers_lockWriter(SharedMutex());
	test_0reader1writer_unlockWriter(SharedMutex());
	test_1reader0writers_lockWriter(SharedMutex());
	test_1reader1writer_blockedReader_unlockWriter(SharedMutex());
	test_1reader1writer_blockedWriter_unlockWriter(SharedMutex());
	test_2readers0writers_lockWriter(SharedMutex());
	test_2readers1writer_blockedReaders_unlockWriter(SharedMutex());
	test_2readers1writer_blockedWriter_unlockWriter(SharedMutex());


	// writers\readers  0   1   2
	//
	// 0				
	// 1			  <-.<->.<->.->
	// 2
	//
	test_0reader1writers_unlockReader(SharedMutex());
	test_0reader1writers_lockReader(SharedMutex());
	test_1reader1writer_readerBlocked_unlockReader(SharedMutex());
	test_1reader1writer_writerBlocked_unlockReader(SharedMutex());
	test_1reader1writer_blockedReader_lockReader(SharedMutex());
	test_1reader1writer_blockedWriter_lockReader(SharedMutex());
	test_2readers1writer_blockedReaders_unlockReader(SharedMutex());
	test_2readers1writer_blockedWriter_unlockReader(SharedMutex());
	test_2readers1writer_blockedReaders_lockReader(SharedMutex());
	test_2readers1writer_blockedWriter_lockReader(SharedMutex());

	// writers\readers  0   1   2
	//
	// 0				
	// 1				^   ^   ^
	// 2				v   v   v
	//
	test_0readers1writers_lockWriter(SharedMutex());
	test_0readers2writers_unlockWriter(SharedMutex());
	test_1reader1writer_blockedReader_lockWriter(SharedMutex());
	test_1reader1writer_blockedWriter_lockWriter(SharedMutex());
	test_1reader2writers_blockedReaderAndWriter_unlockWriter(SharedMutex());
	test_1reader2writers_blockedWriters_unlockWriter(SharedMutex());
	test_2readers1writer_blockedReaders_lockWriter(SharedMutex());
	test_2readers1writer_blockedWriter_lockWriter(SharedMutex());
	test_2readers2writers_blockedReadersAndWriter_unlockWriter(SharedMutex());
	test_2readers2writers_blockedWriters_unlockWriter(SharedMutex());

	// writers\readers  0   1   2
	// 0				
	// 1
	// 2			  <-.<->.<->.->
	test_0readers2writers_unlockReader(SharedMutex());
	test_0readers2writers_lockReader(SharedMutex());
	test_1reader2writers_blockedReaderAndWriter_unlockReader(SharedMutex());
	test_1reader2writers_blockedWriters_unlockReader(SharedMutex());
	test_1reader2writers_blockedReaderAndWriter_lockReader(SharedMutex());
	test_1reader2writers_blockedWriters_lockReader(SharedMutex());
	test_2readers2writers_blockedReadersAndWriter_unlockReader(SharedMutex());
	test_2readers2writers_blockedWriters_unlockReader(SharedMutex());
	test_2readers2writers_blockedReadersAndWriter_lockReader(SharedMutex());
	test_2readers2writers_blockedWriters_lockReader(SharedMutex());

	// writers\readers  0   1   2
	// 0				
	// 1
	// 2			    .   .   .
	//                  v   v   v
	test_0readers2writers_lockWriter(SharedMutex());
	test_1reader2writers_blockedReaderAndWriter_lockWriter(SharedMutex());
	test_1reader2writers_blockedWriters_lockWriter(SharedMutex());
	test_2readers2writers_blockedReadersAndWriter_lockWriter(SharedMutex());
	test_2readers2writers_blockedWriters_lockWriter(SharedMutex());

	system("pause");
	return 0;
}
