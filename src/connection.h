#ifndef COIRC_CONNECTION_H
#define COIRC_CONNECTION_H

#include "main.h"
#include <QObject>
#include <QAbstractSocket>

#define BOLD_STR				"\x02"
#define BOLD_CHAR				'\x02'
#define COLOR_STR				"\x03"
#define COLOR_CHAR			'\x03'
#define NORMAL_STR			"\x0F"
#define NORMAL_CHAR			'\x0F'
#define UNDERLINE_STR		"\x15"
#define UNDERLINE_CHAR		'\x15'
#define REVERSE_STR			"\x16"
#define REVERSE_CHAR			'\x16'

class Context;
class QTcpSocket;
class QTimer;

class IRCConnection : public QObject
{	public:
	enum EConnectionState
		{	EDisconnected,
			EConnecting,
			ERegistering,
			EConnected,
		};

		// =============================================================================
		// IRC server reply codes.. not a comprehensive list
		enum EReplyCode
		{	ERplWelcome             =   1,
			ERplYourHost            =   2,
			ERplCreated             =   3,
			ERplMyInfo              =   4,
			ERplSupported           =   5,
			ERplUniqueId            =  42,
			ERplAway                = 301,
			ERplUserHost            = 302,
			ERplIsOn                = 303,
			ERplUnAway              = 305,
			ERplNowAway             = 306,
			ERplWhoisUser           = 311,
			ERplWhoisServer         = 312,
			ERplWhoisOperator       = 313,
			ERplWhowasUser          = 314,
			ERplEndOfWho            = 315,
			ERplWhoisIdle           = 317,
			ERplEndOfWhois          = 318,
			ERplWhoisChannels       = 319,
			ERplEndOfWhowas         = 369,
			ERplListStart           = 321,
			ERplList                = 322,
			ERplListEnd             = 323,
			ERplChannelModeIs       = 324,
			ERplUniqOpIs            = 325,
			ERplWhoisAccount        = 330,
			ERplNoTopic             = 331,
			ERplTopic               = 332,
			ERplInviting            = 341,
			ERplSummoning           = 342,
			ERplInviteList          = 346,
			ERplEndOfInviteList     = 347,
			ERplExceptList          = 348,
			ERplEndOfExceptList     = 349,
			ERplVersion             = 351,
			ERplWhoReply            = 352,
			ERplNameReply           = 353,
			ERplLinks               = 364,
			ERplEndOfLinks          = 365,
			ERplEndOfNames          = 366,
			ERplBanList             = 367,
			ERplEndOfBanList        = 368,
			ERplInfo                = 371,
			ERplMotd                = 372,
			ERplEndOfInfo           = 374,
			ERplMotdStart           = 375,
			ERplEndOfMotd           = 376,
			ERplYoureOper           = 381,
			ERplRehashing           = 382,
			ERplYoureService        = 383,
			ERplTime                = 391,
			ERplErroneusNickname    = 432,
			ERplNicknameInUse       = 433,
			ERplNeedMoreParams      = 461,
		};

	Q_OBJECT
	DELETE_COPY (IRCConnection)
	NEW_PROPERTY (public,  QString,				Nick)
	NEW_PROPERTY (public,  QString,				User)
	NEW_PROPERTY (public,  QString,				Name)
	NEW_PROPERTY (private, Context*,				Context)
	NEW_PROPERTY (private, QString,				Hostname)
	NEW_PROPERTY (private, quint16,				Port)
	NEW_PROPERTY (private, EConnectionState,	State)
	NEW_PROPERTY (private, QString,				Linework)

	public:
		explicit IRCConnection (QString host, quint16 port, QObject* parent = 0);
		virtual ~IRCConnection();

		void write (QString text);
		void connectToServer();
		void disconnectFromServer (QString quitmessage = "");

		static const QList<IRCConnection*>& getAllConnections();

	public slots:
		void readyRead();
		void writeLogin();

	private:
		QTcpSocket* m_socket;
		QTimer*     m_timer;

		void processMessage (QString msg);
		void print (QString msg, bool allow_internals = true);
		void parseNumeric (QString msg, QStringList tokens, int num);

	private slots:
		void tick();
		void processConnectionError (QAbstractSocket::SocketError err);
};

#endif // COIRC_CONNECTION_H