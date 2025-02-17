#ifndef LibLanc_Blocking_h
#define LibLanc_Blocking_h

#include <stdint.h>

class LancBlocking
{
public:
  /**
   * Setup the lanc instance by assigning Pins to be used.
   * This function also starts the Timer.
   * @param inputPin  The pin tu use to read LANC signals
   * @param outputPin The Pin to use to send LANC signals
   */
  LancBlocking(uint8_t inputPin, uint8_t outputPin);
  /**
   * Setup the Pins
   */
  void begin();
  // char *error_msg();
  /**
   * This function must be called as quickly as possible in order to not loose
   * synchrionization. If the time between two calls is too long we might
   * loose synchronization and thus transmit invalid values. Since the
   * lanc protocol needs approximately 4 repetitions this will most likely cause
   * the commands to not pass through to the camera.
   *
   * @note This function blocks until the command has finished. This is done to ensure
   *       proper timing. This might be improved by using timers and such.
   * @retval  true   Loop ran successfully.  Communcations in sync.
   * @retval  false  Loop failed, communications have been disabled.  Must restart with begin();
   */
  bool loop();
  /**
   * Set the zoom speed of the camera.
   * @param stepSize Zoom in or out. The value must be in the range of [-8..8].
   *                 Negative values zoom out. The value of 0 is silently ignored,
   *                 as it means no zoom and needs no telegram.
   * @retval  true   Zoom command was successfully transmitted to the camera
   * @retval  false  The passed parameter is out of range
   */
  bool Zoom(int8_t stepSize);
  /**
   * Change the manual focus
   * @param far Whether to pusht the Focus farther away or closer
   */
  void Focus(bool far);

  void RecordToggle();
  /**
   * Autofocus
   *
   * @note Maybe we could read whether the Autofocus is set by the return value
   *       and thus change according to the response.
   */
  void AutoFocus();
  /**
   *  AutoIris
   *
   * @note This command is untested,  its the proper code to trigger the iris,
   *       but the camera I had on hand did not support it.
   */
  void AutoIris();
  /**
   *  Iris
   *
   * @note This command is untested,  its the proper code to trigger the open/close
   *       but the camera I had on hand did not support it.
   */
  void Iris(bool moreopen);
  /**
   *  ClearCommand
   *
   * @note Set the next lanc transmission to no command
   */
  void ClearCommand();
  /**
   *  PowerDown
   *
   * @note Instruct the camera to power down.  lanc communications will likley stop on the cameras part after this command completes.
   */
  void PowerDown();
  void VideoCameraSpecialCommand(uint8_t data);
  void VideoCameraNormalCommand(uint8_t data);
  void VideoCameraOpsCommand(uint8_t data);

private:
  /**
   * Transmit a new Camera command.
   * @param data The special Video Camera command that should be transmitted
   *             to the camera
   *
   * @note This function blocks until the command has finished. This is done to ensure
   *       proper timing. This might be improved by using timers and such.
   */
  void setTransmitDataVideoCameraSpecialCommand(uint8_t data);
  void setTransmitDataVideoCameraNormalCommand(uint8_t data);
  void setTransmitDataVideoCameraOpsCommand(uint8_t data);
  /**
   * Optimized fucntion that does the actual transmission/reception.
   * @param transmitReceiveBuffer Buffer that contains the transmission data and
   *                              will contain the received data after the data
   *                              exchange completes
   * @param repeats               Number of transmissions to execute before returning.
   *                              lanc needs 4 Transmissions in order to safely transmit.
   */
  void lancTransmitReceive(uint8_t transmitReceiveBuffer[8], uint8_t repeats);

  unsigned long syncTransmission();
  bool transmitByte(uint8_t byte, unsigned long startTime);
  bool receiveByte(uint8_t *byte, unsigned long startTime);
  unsigned long waitNextStart();
  unsigned long waitForStartBit();
  void waitStartBitComplete(unsigned long startTime);
  void transmitOne();
  void transmitZero();
  bool inputState();
  void delayUsWithStartTime(unsigned long startTime, unsigned long waitTime);

  uint8_t _transmitReceiveBuffer[8];
  uint8_t _inputPin;
  uint8_t _outputPin;
};

#endif // LibLanc_Blocking_h
