                         USER
                           |
                           v
                  +-------------------+
                  |    UI PROCESS     |
                  |      ui.c         |
                  |                   |
                  |  User Commands    |
                  |  Display Output    |
                  +-------------------+
                           |
                           |
                     UI → Core
                    POSIX Message Queue
                           |
                           v
                  +-------------------+
                  |   CORE PROCESS    |
                  |      core2.c      |
                  |                   |
                  |   CPU Execution   |
                  |      Memory       |
                  |  Queue Operations |
                  |  Stack Operations |
                  +-------------------+
                           |
                           |
                    Core → Logger
                   POSIX Message Queue
                           |
                           v
                  +-------------------+
                  |  LOGGER PROCESS   |
                  |     diganth.c     |
                  |                   |
                  |  Message Handling |
                  |  Event Logging    |
                  +-------------------+
                           |
                           v
                  +-------------------+
                  |    NAMED FIFO     |
                  | /tmp/simulator_   |
                  |    log_fifo       |
                  +-------------------+
                           |
                           v
                  +-------------------+
                  |     LOG FILES     |
                  | execution.log     |
                  | error.log         |
                  +-------------------+


              -------------------------------
                    IPC LAYER
              -------------------------------

          UI → Core       : POSIX Message Queue
          Core → Logger   : POSIX Message Queue
          Logger → Logs   : Named FIFO
