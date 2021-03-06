/**
 * @file detach.cpp
 * @author Sina Karvandi (sina@rayanfam.com)
 * @brief .detach command
 * @details
 * @version 0.1
 * @date 2020-08-28
 *
 * @copyright This project is released under the GNU Public License v3.
 *
 */
#include "pch.h"

//
// Global Variables
//
extern BOOLEAN g_IsAttachedToUsermodeProcess;

/**
 * @brief help of .detach command
 *
 * @return VOID
 */
VOID CommandDetachHelp() {
  ShowMessages(".detach : detach from debugging a user-mode process.\n\n");
  ShowMessages("syntax : \t.detach\n");
}

/**
 * @brief .detach command handler
 *
 * @param SplittedCommand
 * @param Command
 * @return VOID
 */
VOID CommandDetach(vector<string> SplittedCommand) {

  BOOLEAN Status;
  ULONG ReturnedLength;
  DEBUGGER_ATTACH_DETACH_USER_MODE_PROCESS DetachRequest = {0};

  if (SplittedCommand.size() >= 2) {
    ShowMessages("incorrect use of '.detach'\n\n");
    CommandDetachHelp();
    return;
  }

  //
  // Check if debugger is loaded or not
  //
  if (!g_DeviceHandle) {
    ShowMessages("Handle not found, probably the driver is not loaded. Did you "
                 "use 'load' command?\n");
    return;
  }

  //
  // We wanna attach to a remote process
  //
  DetachRequest.IsAttach = FALSE;

  //
  // Send the request to the kernel
  //

  Status = DeviceIoControl(
      g_DeviceHandle,                                 // Handle to device
      IOCTL_DEBUGGER_ATTACH_DETACH_USER_MODE_PROCESS, // IO Control
                                                      // code
      &DetachRequest,                                 // Input Buffer to driver.
      SIZEOF_DEBUGGER_ATTACH_DETACH_USER_MODE_PROCESS, // Input buffer length
      &DetachRequest, // Output Buffer from driver.
      SIZEOF_DEBUGGER_ATTACH_DETACH_USER_MODE_PROCESS, // Length of output
                                                       // buffer in bytes.
      &ReturnedLength, // Bytes placed in buffer.
      NULL             // synchronous call
  );

  if (!Status) {
    ShowMessages("ioctl failed with code 0x%x\n", GetLastError());
    return;
  }

  //
  // Check if attaching was successful then we can set the attached to true
  //
  if (DetachRequest.Result == DEBUGEER_OPERATION_WAS_SUCCESSFULL) {
    g_IsAttachedToUsermodeProcess = FALSE;
  }
}
