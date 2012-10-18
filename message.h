
struct message
{
	int fd;
	bool finish;
	message()
	{}
	message(int fd,	bool finish)
	{
		this->fd = fd;
		this->finish = finish;
	}
};
