#pragma once

#include "fetch_event.h"
#include "archive/poll.h"

namespace core::wim
{

class fetch_event_poll_update : public fetch_event
{
public:
    fetch_event_poll_update() = default;

    virtual int32_t parse(const rapidjson::Value& _node_event_data) override;
    virtual void on_im(std::shared_ptr<core::wim::im> _im, std::shared_ptr<auto_callback> _on_complete) override;

    const archive::poll_data& get_poll() const;

private:
    archive::poll_data poll_;
};

}
