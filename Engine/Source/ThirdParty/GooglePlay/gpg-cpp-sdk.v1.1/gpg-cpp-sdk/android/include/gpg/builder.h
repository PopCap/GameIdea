/**
 * @file gpg/builder.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Used to construct a GameServices object.
 */

#ifndef GPG_BUILDER_H_
#define GPG_BUILDER_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <functional>
#include <memory>
#include <string>
#include "gpg/common.h"
#include "gpg/game_services.h"
#include "gpg/multiplayer_invitation.h"
#include "gpg/platform_configuration.h"
#include "gpg/quest.h"
#include "gpg/turn_based_match.h"

namespace gpg {

class BuilderImpl;
enum class LogLevel;  // Defined in gpg/types.h

/**
 * Used for creating and configuring an instance of the GameServices class.
 */
class GPG_EXPORT GameServices::Builder {
 public:
  Builder();
  ~Builder();

  /**
   * Specifies the platform for which Builder will create the GameServices
   * object. For more information, see the documentation on platform
   * configuration:
   * https://developers.google.com/games/services/cpp/api/platform__configuration_8h
   */
  std::unique_ptr<GameServices> Create(PlatformConfiguration const &platform);

  /**
   * The type of logging callback that can be provided to the SDK.
   * @ingroup Callbacks
   */
  typedef std::function<void(LogLevel, std::string const &)> OnLogCallback;

  /**
   * DEPRECATED: Prefer SetOnLog and SetDefaultOnLog.
   * Registers a callback which will be used to perform logging.
   * min_level specifies the minimum log level at which the callback is invoked.
   * Possible levels are: VERBOSE, INFO, WARNING, and ERROR.
   */
  Builder &SetLogging(OnLogCallback callback, LogLevel min_level)
      GPG_DEPRECATED;

  /**
   * DEPRECATED: Prefer SetOnLog and SetDefaultOnLog.
   * Registers a callback which will be used to perform logging.
   * the same as calling SetLogging(OnLogCallback, LogLevel) with a LogLevel of
   * INFO.
   */
  Builder &SetLogging(OnLogCallback callback) GPG_DEPRECATED;

  /**
   * Registers a callback which will be used to perform logging.
   * min_level specifies the minimum log level at which the callback is invoked.
   * Possible levels are: VERBOSE, INFO, WARNING, and ERROR.
   */
  Builder &SetOnLog(OnLogCallback callback, LogLevel min_level);

  /**
   * Registers a callback which will be used to perform logging.
   * This is equivalent to calling SetOnLog(OnLogCallback, LogLevel) with a
   * LogLevel of INFO.
   */
  Builder &SetOnLog(OnLogCallback callback);

  /**
   * Specifies that logging should use the DEFAULT_ON_LOG_CALLBACK at the
   * specified log level. min_level specifies the minimum log level at which the
   * default callback is invoked.
   * Possible levels are: VERBOSE, INFO, WARNING, and ERROR.
   * This is equivalent to calling SetOnLog(OnLogCallback, LogLevel) with
   * OnLogCallback set to DEFAULT_ON_LOG_CALLBACK and a LogLevel of min_level.
   */
  Builder &SetDefaultOnLog(LogLevel min_level);

  /**
   * The type of the authentication action started callback that can be provided
   * to the SDK.
   * @ingroup Callbacks
   */
  typedef std::function<void(AuthOperation)> OnAuthActionStartedCallback;

  /**
   * Registers a callback to be called when authorization has begun.
   */
  Builder &SetOnAuthActionStarted(OnAuthActionStartedCallback callback);

  /**
   * The type of authentication action finished callback that can be provided to
   * the SDK.
   * @ingroup Callbacks
   */
  typedef std::function<void(AuthOperation, AuthStatus)>
      OnAuthActionFinishedCallback;

  /**
   * Registers a callback to be called when authorization has finished.
   */
  Builder &SetOnAuthActionFinished(OnAuthActionFinishedCallback callback);
  /**
   * The type of the multiplayer invitation callback that can be provided to the
   * SDK. The MultiplayerInvitation is only Valid() on UPDATED events.
   * @ingroup Callbacks
   */
  typedef std::function<
      void(TurnBasedMultiplayerEvent, std::string, MultiplayerInvitation)>
          OnMultiplayerInvitationEventCallback;

  /**
   * Registers a callback to be called when an event occurrs for a multiplayer
   * invitation.
   */
  Builder &SetOnMultiplayerInvitationEvent(
      OnMultiplayerInvitationEventCallback callback);

  /**
   * The type of the turn based multiplayer event callback that can be provided
   * to the SDK. The TurnBasedMatch parameter is only Valid() on UPDATED events.
   * @ingroup Callbacks
   */
  typedef std::function<
      void(TurnBasedMultiplayerEvent event, std::string, TurnBasedMatch)>
          OnTurnBasedMatchEventCallback;

  /**
   * Registers a callback to be called when an event occurrs for a turn based
   * multiplayer match.
   */
  Builder &SetOnTurnBasedMatchEvent(
      OnTurnBasedMatchEventCallback callback);
  /**
   * The type of the quest completed callback that can be provided to the
   * SDK. Provides the completed quest.
   * @ingroup Callbacks
   */
  typedef std::function<void(Quest quest)> OnQuestCompletedCallback;

  /**
   * Registers a callback to be called when a quest changes to the state
   * QuestState::COMPLETED.
   */
  Builder &SetOnQuestCompleted(OnQuestCompletedCallback callback);
  /**
   * Enable Snapshots. This is equivalent to
   * <code>AddOauthScope(kSnapshotScope)</code>.
   * See {@link SnapshotManager} for more details.
   */
  Builder &EnableSnapshots();

  /**
   * Scopes beyond the required Play Games scope to request.
   * Details on authorization scopes at
   * https://developers.google.com/+/api/oauth#scopes.
   */
  Builder &AddOauthScope(std::string const &scope);
 private:
  Builder(Builder const &) = delete;
  Builder &operator=(Builder const &) = delete;

  std::unique_ptr<BuilderImpl> impl_;
};

}  // namespace gpg

#endif  // GPG_BUILDER_H_
