#pragma once
#include <string>
#ifndef SLEEPY_ONE_THREAD
#include <thread>
#include <condition_variable>
#endif
#include <memory>
#include <unordered_map>
#include <functional>

//objects
#include "message.h"
#include "channel.h"
#include "server.h"
#include "invite.h"
#include "webhook.h"
#include "permissions.h"
#include "gateway.h"
#include "voice.h"

#include "error.h"
#include "session.h"
#include "common_return_types.h"

/*

remember to make function perimeters const

*/

namespace SleepyDiscord {
#define TOKEN_SIZE 64
	class BaseDiscordClient {
	public:
		BaseDiscordClient() {}
		BaseDiscordClient(const std::string _token) { start(_token); }
		~BaseDiscordClient();

		Response request(const RequestMethod method, const std::string url, const std::string jsonParameters = ""/*,
			cpr::Parameters httpParameters = cpr::Parameters{}*/, const std::initializer_list<Part>& multipartParameters = {});
		Response request(const RequestMethod method, const std::string url, const std::initializer_list<Part>& multipartParameters);
		/*Response request(const RequestMethod method, std::string url, cpr::Parameters httpParameters);*/

		const std::string path(const char* source, std::initializer_list<std::string> values = {});

		void testFunction(std::string teststring);

		//channel functions
		Channel getChannel(Snowflake<Channel> channelID);                                                                           //to do test this
		Channel editChannel(Snowflake<Channel> channelID, std::string name = "", std::string topic = "");	                          //to do test this
		Channel editChannelName(Snowflake<Channel> channelID, std::string name);                                                    //to do test this
		Channel editChannelTopic(Snowflake<Channel> channelID, std::string topic);                                                  //to do test this
		Channel deleteChannel(Snowflake<Channel> channelID);
		enum GetMessagesKey {na, around, before, after, limit};
		json::ArrayWrapper<Message> getMessages(Snowflake<Channel> channelID, GetMessagesKey when, Snowflake<Message> messageID, uint8_t limit = 0);
		Message getMessage(Snowflake<Channel> channelID, Snowflake<Message> messageID);                                                    //to do test this, and add more then one message return
		ObjectResponse<Message> sendMessage(Snowflake<Channel> channelID, std::string message, bool tts = false);
		Message uploadFile(Snowflake<Channel> channelID, std::string fileLocation, std::string message);                             //to do test this
		bool addReaction(Snowflake<Channel> channelID, Snowflake<Message> messageID, std::string emoji);
		bool removeReaction(Snowflake<Channel> channelID, Snowflake<Message> messageID, std::string emoji, Snowflake<User> userID = "@me");  //to do test this
		std::vector<Reaction> getReactions(Snowflake<Channel> channelID, Snowflake<Message> messageID, std::string emoji);                //to do test this
		void removeAllReactions(Snowflake<Channel> channelID, Snowflake<Message> messageID);                                              //to do test this
		Message editMessage(Snowflake<Channel> channelID, Snowflake<Message> messageID, std::string newMessage);
		bool deleteMessage(Snowflake<Channel> channelID, Snowflake<Message> messageID);
		bool bulkDeleteMessages(Snowflake<Channel> channelID, std::vector<Snowflake<Message>> messageIDs);
		/*allow is a bitwise value of all allowed permissions
		deny is a bitwise value of all deisallowed permissions
		type is "member" for a user or "role" for a role*/
		bool editChannelPermissions(Snowflake<Channel> channelID, std::string ID, int allow, int deny, std::string type);           //to do test this
		std::vector<Invite> getChannelInvites(Snowflake<Channel> channelID);                                                        //to do test this
		Invite createChannelInvite(Snowflake<Channel> channelID, const uint64_t maxAge = 0, const uint64_t maxUses = 0, const bool temporary = false, const bool unique = false);  //to do test this
		bool removeChannelPermission(Snowflake<Channel> channelID, std::string ID);
		bool sendTyping(Snowflake<Channel> channelID);                                                                              //to do test this
		std::vector<Message> getPinnedMessages(Snowflake<Channel> channelID);                                                       //to do test this
		bool pinMessage(Snowflake<Channel> channelID, Snowflake<Message> messageID);
		bool unpinMessage(Snowflake<Channel> channelID, Snowflake<Message> messageID);
		void addRecipient(Snowflake<Channel> channelID, Snowflake<User> userID);
		void removeRecipient(Snowflake<Channel> channelID, Snowflake<User> userID);
		//IntelliSense Help
		/*functions with more then one name to make life easy for users that use IntelliSense*/
		inline bool deleteReaction(Snowflake<Channel> channelID, Snowflake<Message> messageID, std::string emoji) { return removeReaction(channelID, messageID, emoji); }
		inline void deleteAllReactions(Snowflake<Channel> channelID, Snowflake<Message> messageID) { removeAllReactions(channelID, messageID); }
		inline bool deleteChannelPermission(Snowflake<Channel> channelID, std::string ID) { return removeChannelPermission(channelID, ID); }
		inline void deleteRecipient(Snowflake<Channel> channelID, Snowflake<User> userID) { removeRecipient(channelID, userID); }
		//Same Name Convenience
		/*functions that dose the same thing as the function with the same name, but
		  uses different variable types for the sake of convenience*/
		inline Message editMessage(Message message, std::string newMessage) { return editMessage(message.channelID, message.ID, newMessage); }

		//server functions
		//Server createServer(std::string name, std::string region, std::string icon, int verificationLevel, int defaultMessageNotifications, std::vector<Role> roles, std::vector<>) what's a create guild channel body?
		Server getServer(Snowflake<Server> serverID);                                                                              //to do test this
		//edit Server		//ask discord api server about what the default values should be
		Server deleteServer(Snowflake<Server> serverID);                                                                           //to do test this
		json::ArrayWrapper<Channel> GetServerChannels(Snowflake<Server> serverID);                                                        //to do test this
		Channel createTextChannel(Snowflake<Server> serverID, std::string name);	                                                  //to do test this
		std::vector<Channel> editChannelPositions(Snowflake<Server> serverID, std::vector<std::pair<std::string, uint64_t>> positions);         //to do test this
		ServerMember getMember(Snowflake<Server> serverID, Snowflake<User> userID);                                                   //to do test this
		std::vector<ServerMember> listMembers(Snowflake<Server> serverID, uint16_t limit = 0, std::string after = "");             //to do test this
		ServerMember addMember(Snowflake<Server> serverID, Snowflake<User> userID, std::string accesToken, std::string nick = "", std::vector<Role> roles = {}, bool mute = false, bool deaf = false); //to do test this
		//edit member needs array maker
		bool muteServerMember(Snowflake<Server> serverID, Snowflake<User> userID, bool mute = true);                                  //to do test this
		bool editNickname(Snowflake<Server> serverID, std::string newNickname);
		bool addRole(Snowflake<Server> serverID, Snowflake<User> userID, Snowflake<Role> roleID);
		bool removeRole(Snowflake<Server> serverID, Snowflake<User> userID, Snowflake<Role> roleID);   //removes role from member
		bool kickMember(Snowflake<Server> serverID, Snowflake<User> userID);
		std::vector<User> getBans(Snowflake<Server> serverID);                                                                     //to do test this
		bool banMember(Snowflake<Server> serverID, Snowflake<User> userID);                                                         //to do test this later
		bool unbanMember(Snowflake<Server> serverID, Snowflake<User> userID);                                                       //to do test this later
		std::vector<Role> getRoles(Snowflake<Server> serverID);                                                                    //to do test this
		Role createRole(Snowflake<Server> serverID, std::string name = "", Permission permissions = NONE, unsigned int color = 0, bool hoist = false, bool mentionable = false);//to do test this
		std::vector<Role> editRolePosition(Snowflake<Server> serverID, std::vector<std::pair<std::string, uint64_t>> positions);                //to do test this
		std::string editRole(Snowflake<Server> serverID, Snowflake<Role> roleID, std::string name = "", Permission permissions = NONE, uint32_t color = 1 << 24, int position = -1, uint8_t hoist = 2, uint8_t mentionable = 2);	//to do test
		bool deleteRole(Snowflake<Server> serverID, Snowflake<Role> roleID);                                                          //to do test this
		//get prune count	needs testing to know what object they are talking about
		void pruneMembers(Snowflake<Server> serverID, const unsigned int numOfDays);                                               //to do test
		std::vector<VoiceRegion> getVoiceRegions();  //needs voice region class
		std::vector<Invite> getServerInvites(Snowflake<Server> serverID);
		std::string getIntegrations(Snowflake<Server> serverID);    //needs whatever a integration class is                        //to do test
		bool createIntegration(Snowflake<Server> serverID, std::string type, std::string integrationID);                          //to do test
		bool editIntergration(Snowflake<Server> serverID, std::string integrationID, int expireBegavior, int expireGracePeriod, bool enbleEmoticons); //to do test
		bool deleteIntegration(Snowflake<Server> serverID, std::string integrationID);                                            //to do test this
		bool syncIntegration(Snowflake<Server> serverID, std::string integrationID);                                              //to do test this
		ServerEmbed getServerEmbed(Snowflake<Server> serverID);
		//edit server embed   I don't know what the perms are

		//Invite functions
		Invite inviteEndpoint(RequestMethod method, std::string inviteCode);
		Invite getInvite(std::string inviteCode);                                                                             //to do test this
		Invite deleteInvite(std::string inviteCode);                                                                          //to do test this
		Invite acceptInvite(std::string inviteCode);	//not available to bot accounts                                       //to do test this

		//User functions
		User getCurrentUser();                                                                                                //to do test this
		User getUser(Snowflake<User> userID);                                                                                    //to do test this
		//User editCurrentUser();		//needs Avatar data thing?
		json::ArrayWrapper<Server> getServers();
		bool leaveServer(Snowflake<Server> serverID);                                                                              //to do test this
		std::vector<DMChannel> getDirectMessageChannels();
		DMChannel createDirectMessageChannel(std::string recipientID);
		//DMChannel createGroupDirectMessageChannel(std:vector<std::string> accessTokens, )   what is a dict???
		std::vector<Connection> getUserConnections();

		//Voice Functions
		//getVoiceRegions

		//Webhook functions
		Webhook createWebhook(Snowflake<Channel> channelID, std::string name, std::string avatar = "");                             //to do test this
		std::vector<Webhook> getChannelWebhooks(Snowflake<Channel> channelID);                                                      //to do test this
		std::vector<Webhook> getServerWebhooks(Snowflake<Server> serverID);                                                        //to do test this
		Webhook getWebhook(Snowflake<Webhook> webhookID, std::string webhookToken = "");                                            //to do test this
		Webhook editWebhook(Snowflake<Webhook> webhookID, std::string webhookToken = "", std::string name = "", std::string avatar = "");    //you can leave token or name as null //to do test this
		bool deleteWebhook(Snowflake<Webhook> webhookID, std::string webhookToken = "");
		Webhook requestExecuteWebhook(Snowflake<Webhook> webhookID, std::string webhookToken, std::pair<std::string, std::string> pair, bool wait, std::string username, std::string avatar_url, bool tts);     //note: it's possiable to have both a file and embeds
		Webhook executeWebhook(Snowflake<Webhook> webhookID, std::string webhookToken, std::string content, bool wait = false, std::string username = "", std::string avatar_url = "", bool tts = false);       //to do test this
		Webhook executeWebhook(Snowflake<Webhook> webhookID, std::string webhookToken, std::vector<Embed> embeds, bool wait = false, std::string username = "", std::string avatar_url = "", bool tts = false); //to do test this
		Webhook executeWebhook(Snowflake<Webhook> webhookID, std::string webhookToken, filePathPart file, bool wait = false, std::string username = "", std::string avatar_url = "", bool tts = false);         //to do test this

		//websocket functions
		void updateStatus(std::string gameName = "", uint64_t idleSince = 0);

		void waitTilReady();
		const bool isReady() { return ready; }
		const bool isBot() { return bot; }
		const bool isRateLimited() { return messagesRemaining <= 0 || request(Get, "gateway").statusCode == TOO_MANY_REQUESTS; }
		void quit();	//public function for diconnecting
		virtual void run();
	protected:
		/* list of events
		READY
		RESUMED
		GUILD_CREATE
		GUILD_DELETE
		GUILD_UPDATE
		GUILD_BAN_ADD
		GUILD_BAN_REMOVE
		GUILD_MEMBER_ADD
		GUILD_MEMBER_REMOVE
		GUILD_MEMBER_UPDATE
		GUILD_ROLE_CREATE
		GUILD_ROLE_DELETE
		GUILD_ROLE_UPDATE
		GUILD_EMOJIS_UPDATE
		GUILD_MEMBERS_CHUNK
		CHANNEL_CREATE
		CHANNEL_DELETE
		CHANNEL_UPDATE
		CHANNEL_PINS_UPDATE
		PRESENCE_UPDATE
		USER_UPDATE
		USER_NOTE_UPDATE
		USER_SETTINGS_UPDATE
		VOICE_STATE_UPDATE
		TYPING_START
		MESSAGE_CREATE
		MESSAGE_DELETE
		MESSAGE_UPDATE
		MESSAGE_DELETE_BULK
		VOICE_SERVER_UPDATE
		GUILD_SYNC
		RELATIONSHIP_ADD
		RELATIONSHIP_REMOVE
		MESSAGE_REACTION_ADD
		MESSAGE_REACTION_REMOVE
		MESSAGE_REACTION_REMOVE_ALL
		*/
		virtual void onReady             (std::string* jsonMessage);
		virtual void onResumed           (std::string* jsonMessage);
		virtual void onDeleteServer      (std::string* jsonMessage);
		virtual void onEditServer        (std::string* jsonMessage);
		virtual void onBan               (std::string* jsonMessage);
		virtual void onUnban             (std::string* jsonMessage);
		virtual void onMember            (std::string* jsonMessage);
		virtual void onRemoveMember      (std::string* jsonMessage);
		virtual void onDeleteMember      (std::string* jsonMessage);
		virtual void onEditMember        (std::string* jsonMessage);
		virtual void onRole              (std::string* jsonMessage);
		virtual void onDeleteRole        (std::string* jsonMessage);
		virtual void onEditRole          (std::string* jsonMessage);
		virtual void onEditEmojis        (std::string* jsonMessage);
		virtual void onMemberChunk       (std::string* jsonMessage);
		virtual void onDeleteChannel     (std::string* jsonMessage);
		virtual void onEditChannel       (std::string* jsonMessage);
		virtual void onPinMessages       (std::string* jsonMessage);
		virtual void onPresenceUpdate    (std::string* jsonMessage);
		virtual void onEditUser          (std::string* jsonMessage);
		virtual void onEditUserNote      (std::string* jsonMessage);
		virtual void onEditUserSettings  (std::string* jsonMessage);
		virtual void onEditVoiceState    (std::string* jsonMessage);
		virtual void onTyping            (std::string* jsonMessage);
		virtual void onDeleteMessage     (std::string* jsonMessage);
		virtual void onEditMessage       (std::string* jsonMessage);
		virtual void onBulkDelete        (std::string* jsonMessage);
		virtual void onEditVoiceServer   (std::string* jsonMessage);
		virtual void onServerSync        (std::string* jsonMessage);
		virtual void onRelationship      (std::string* jsonMessage);
		virtual void onRemoveRelationship(std::string* jsonMessage);
		virtual void onDeleteRelationship(std::string* jsonMessage);
		virtual void onReaction          (std::string* jsonMessage);
		virtual void onRemoveReaction    (std::string* jsonMessage);
		virtual void onDeleteReaction    (std::string* jsonMessage);
		virtual void onRemoveAllReaction (std::string* jsonMessage);
		virtual void onDeleteAllReaction (std::string* jsonMessage);
		virtual void onMessage           (Message message         );
		virtual void onEditedMessage     (std::string* jsonMessage);
		virtual void onServer            (Server server           );
		virtual void onChannel           (std::string* jsonMessage);
		virtual void onEditedRole        (std::string* jsonMessage);
		virtual void onDispatch          (std::string* jsonMessage);

		//websocket stuff
		virtual void onHeartbeat();
		virtual void onHeartbeatAck();
		virtual void onInvaldSession();
		virtual void onDisconnect();
		virtual void onResume();

		virtual void onQuit();
		virtual void onResponse(Response response);
		virtual void sleep(const unsigned int milliseconds);
		virtual void fileRead(const char* path, std::string*const file);
		virtual void tick(float deltaTime);
		virtual void onError(ErrorCode errorCode, const std::string errorMessage);

		/*do not use or overwrite the protected values below,
		unless you know what you are doing*/
		void processMessage(std::string message);
		void resumeHeartbeatLoop();
		inline std::string getToken() { return *token.get(); }
		void start(const std::string _token, const char maxNumOfThreads = 2);
		virtual bool connect(const std::string & uri) { return false; }
		virtual void send(std::string message) {}
		virtual void disconnect(unsigned int code, const std::string reason) {}
		virtual void runAsync();
	private:
		int heartbeatInterval = 0;
		int64_t nextHeartbeat = 0;
		int lastSReceived = 0;
		bool wasHeartbeatAcked = true;

		enum OPCode {
			DISPATCH              = 0,		//dispatches an event
			HEARTHBEAT            = 1,		//used for ping checking
			IDENTIFY              = 2,		//used for client handshake
			STATUS_UPDATE         = 3,		//used to update the client status
			VOICE_STATE_UPDATE    = 4,	 	//used to join / move / leave voice channels
			VOICE_SERVER_PING     = 5,	 	//used for voice ping checking
			RESUME                = 6,		//used to resume a closed connection
			RECONNECT             = 7,		//used to tell clients to reconnect to the gateway
			REQUEST_GUILD_MEMBERS = 8,		//used to request guild members
			INVALID_SESSION       = 9,		//used to notify client they have an invalid session id
			HELLO                 = 10,		//sent immediately after connecting, contains heartbeat and server debug information
			HEARTBEAT_ACK         = 11,		//sent immediately following a client heartbeat that was received
		};

#ifndef SLEEPY_ONE_THREAD
		std::thread clock_thread;
		std::condition_variable* condition = nullptr;
		void runClock_thread();
#endif

		std::unique_ptr<std::string> token;		//stored in a unique_ptr so that you can't see it in the debugger
		std::string sessionID;
		void getTheGateway();
		char theGateway[32];
		bool ready;
		bool bot;
		void sendIdentity();
		void sendResume();
		//bool restart();		//it's like start but when it already started. it's basicly useless in it's current form
		void reconnect(const unsigned int status = 1000);
		void disconnectWebsocket(unsigned int code, const std::string reason = "");
		bool sendL(std::string message);    //the L stands for Limited
		int64_t nextHalfMin = 0;

		//rate limiting
		int8_t messagesRemaining;

		//error handling
		void setError(int errorCode);

		//templates for discord objects
		template <class _DiscordObject>
		_DiscordObject request(const RequestMethod method, std::string _url, std::string jsonParameters = "") {
			std::string response = request(method, _url, jsonParameters).text;
			return _DiscordObject(&response);
		}

		template <class _DiscordObject>
		std::vector<_DiscordObject> requestVector(const RequestMethod method, std::string _url, std::string jsonParameters = "") {
			const std::string response = request(method, _url, jsonParameters).text;
			return JSON_getArray<_DiscordObject>(&response);
		}

		//events

		//time
		const int64_t getEpochTimeMillisecond();

		//for endpoint functions
		const std::string getEditPositionString(const std::vector<std::pair<std::string, uint64_t>> positions);
	};
}

//locks away functions that users shouldn't be using
#define SLEEPY_LOCK_CLIENT_FUNCTIONS private:                                      \
                                     using BaseDiscordClient::processMessage;      \
                                     using BaseDiscordClient::resumeHeartbeatLoop; \
                                     using BaseDiscordClient::start;

//This comment stops a warning