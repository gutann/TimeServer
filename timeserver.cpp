#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>


using namespace boost::asio;
//using namespace std;
using boost::system::error_code;
using ip::tcp;

struct CHelloWorld_Service
{
	boost::array<char, 1> buf;

        CHelloWorld_Service(io_service &iosev)
                :m_iosev(iosev),m_acceptor(iosev, tcp::endpoint(tcp::v4(), 1000))
        {}

        void start()
        {
                // 開始等待連接（非阻塞）
                boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
                // 觸發的事件只有error_code參數，所以用boost::bind把socket綁定進去
                m_acceptor.async_accept(*psocket,
                        boost::bind(&CHelloWorld_Service::accept_handler, this, psocket, _1));
        }

        // 有客戶端連接時accept_handler觸發
        void accept_handler(boost::shared_ptr<tcp::socket> psocket, error_code ec)
        {
            if(ec) return;
            // 繼續等待連接
            start();
            // 顯示遠程IP
            std::cout << psocket->remote_endpoint().address() << std::endl;
            // 發送信息(非阻塞)
            //boost::shared_ptr<std::string> pstr(new std::string("ACK"));
            //psocket->async_write_some(buffer(*pstr),
            //boost::bind(&CHelloWorld_Service::write_handler, this, pstr, _1, _2));
     		for (;;)
    		{
      			boost::array<char, 128> buf;
      			boost::system::error_code error;
#if 1
      			size_t len = psocket->read_some(boost::asio::buffer(buf), error);   
				if (error == boost::asio::error::eof)
       				 break; // Connection closed cleanly by peer.
      			else if (error)
        			throw boost::system::system_error(error); // Some other error.
			//sleep()0;
      			std::cout.write(buf.data(), len);
				std::cout << std::endl <<  std::flush;
			 	if (len > 0)
#endif
				{	
					boost::shared_ptr<std::string> pstr(new std::string("ACK"));
               //	psocket->async_write_some(buffer(*pstr),boost::bind(&CHelloWorld_Service::write_handler, this, pstr, _1, _2));
    				boost::asio::async_write(*psocket, buffer(*pstr),
					boost::asio::transfer_at_least(1),
        			boost::bind(&CHelloWorld_Service::write_handler, this, pstr,
          				boost::asio::placeholders::error, 
          				boost::asio::placeholders::bytes_transferred));
				}
			}
#if 0
		boost::asio::streambuf sb;
           // 	while (0 == boost::asio::read(*psocket, sb, ec)) 
		{
			boost::asio::async_read(*psocket, boost::asio::buffer(buf),boost::bind(&CHelloWorld_Service::read_handler, this, boost::asio::placeholders::error));
		//	boost::asio::read(*psocket, sb, ec);
                	std::cout << "received: '" << &sb << std::endl << std::flush;
                	if (ec) 
			{
                    	    std::cout << "status: " << ec.message() << "\n";
                    	    //break;
			}
                }

//		boost::shared_ptr<std::string> pstr(new std::string("ACKK"));
  //              psocket->async_write_some(buffer(*pstr),boost::bind(&CHelloWorld_Service::write_handler, this, pstr, _1, _2));
#endif
	}

        // 異步寫操作完成後write_handler觸發
        void write_handler(boost::shared_ptr<std::string> pstr, error_code ec,
                size_t bytes_transferred)
        {
                if(ec)
                std::cout<< "發送失敗!" << std::endl << std::flush;
                else
                std::cout<< *pstr << " 已發送2 " << std::endl << std::flush;
        }
	
	void read_handler(const boost::system::error_code& error)
	{
    		if(!error)
    		{
        		std::cout << "Message: " << buf.data() << std::endl;
    		}
    		else
    		{
        		std::cout << "Error occurred." << std::endl;
    		}
	}

        private:
                io_service &m_iosev;
                ip::tcp::acceptor m_acceptor;
};

int main(int argc, char* argv[])
{
        io_service iosev;
        CHelloWorld_Service sev(iosev);
        // 開始等待連接
        sev.start();
        iosev.run();

        return 0;
}

