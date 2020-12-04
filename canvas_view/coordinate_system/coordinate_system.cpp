#include "coordinate_system.h"

CoordinateSystem::CoordinateSystem()
{
    scale_ = 1.0f;
    offset_y_ = 0.0f;
}

void CoordinateSystem::SetWorldSize(double x, double y)
{
    world_[0] = x;
    world_[1] = y;

    Rescale();
}


void CoordinateSystem::SetDisplaySize(double x, double y)
{
    display_[0] = x;
    display_[1] = y;

    Rescale();
}

void CoordinateSystem::Rescale()
{
    scale_ = display_[0] / world_[0];
}

double CoordinateSystem::GetSacle()
{
    return scale_;
}

void CoordinateSystem::SetOffset(double offset)
{
    offset_y_ = offset;
}

double CoordinateSystem::GetOffset()
{
    return offset_y_;
}



void CoordinateSystem::GetDisplaySize(double display[2])
{
    display[0] = scale_*world_[0];
    display[1] = scale_*world_[1];
}

void CoordinateSystem::GetWorldSize(double world[2])
{
    world[0] = world_[0];
    world[1] = world_[1];
}

void CoordinateSystem::ComputeWorldToDisplay(double x, double y, double display[2])
{
    display[0] = scale_*x;
    display[1] = scale_*y;
}


void CoordinateSystem::ComputeDisplayToWorld(double x, double y, double world[2])
{
    world[0] = x / scale_;
    world[1] = y / scale_;
}
