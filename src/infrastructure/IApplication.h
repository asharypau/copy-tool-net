#ifndef IAPPLICATION_H
#define IAPPLICATION_H

class IApplication
{
public:
    virtual ~IApplication() = default;

    /**
     * @brief Runs the application.
     */
    virtual void run() = 0;
};

#endif  // IAPPLICATION_H
