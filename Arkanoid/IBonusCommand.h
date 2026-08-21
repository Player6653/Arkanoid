#pragma once

// Паттерн "Команда": каждый временный бонус умеет включить свой эффект (execute) и вернуть всё как было по истечении времени (undo)
class IBonusCommand {
public:
    virtual ~IBonusCommand() = default;

    virtual void execute() = 0;
    virtual void undo() = 0;

    // true для одноразовых команд без длительности (например, "Динамит") — BonusManager тогда не заводит на них таймер и не хранит их для undo().
    virtual bool isInstant() const
    {
        return false;
    }
};
