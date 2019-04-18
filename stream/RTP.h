#pragma once
#include "base/TcpConnection.h"
#include "base/Callback.h"
namespace base {




	class RTP
	{
	public:
		typedef struct
		{
			unsigned char csrc_len : 4;        /**//* expect 0 */

			unsigned char extension : 1;        /**//* expect 1, see RTP_OP below */

			unsigned char padding : 1;        /**//* expect 0 */

			unsigned char version : 2;        /**//* expect 2 */

			/**//* byte 1 */

			unsigned char payload : 7;        /**//* RTP_PAYLOAD_RTSP */

			unsigned char marker : 1;        /**//* expect 1 */

			/**//* bytes 2, 3 */

			unsigned short seq_no;

			/**//* bytes 4-7 */

			unsigned  long timestamp;

			/**//* bytes 8-11 */

			unsigned long ssrc;           

		} RTP_FIXED_HEADER;


		static unsigned int SendNaluPacket(const unsigned char* sliceData, int sliceSize, unsigned  long  pts, TcpConnectionPtr tcpconn);


	private:

	};


}