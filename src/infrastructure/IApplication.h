#ifndef IAPPLICATION_H
#define IAPPLICATION_H

class IApplication
{
public:
    /**
     * @brief Executes the application.
     */
    virtual void run() = 0;

    virtual ~IApplication() = default;
};

#endif  // IAPPLICATION_H
