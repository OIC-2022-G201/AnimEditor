#include "SequenceItem.h"

SequenceItem::SequenceItem(std::string name): name_(std::move(name))
{}

void SequenceItem::Insert(uint32_t frame, asset::ImageAsset image)
{
  if (image_paths_.empty()) frame = 0;
    auto set_result = image_paths_.insert_or_assign(frame, image);
  SetImage(frame, image);
}

void SequenceItem::Erase(uint32_t frame)
{
  image_paths_.erase(frame);
  size_t d = std::distance(image_paths_.begin(), image_paths_.find(frame));
  images_view_.Remove({d, 0});
}

void SequenceItem::SetEndFrame(const std::optional<uint32_t> end_frame)
{
    end_frame_ = end_frame;
}

void SequenceItem::SetLoop(const bool is_loop)
{ is_loop_ = is_loop; }

bool SequenceItem::GetLoop()
{ return is_loop_; }


uint32_t SequenceItem::GetEndFrame() const
{
    if (image_paths_.empty()) return 0;

    return end_frame_.value_or(image_paths_.rbegin()->first);
}

void SequenceItem::SetName(std::string name)
{ name_ = name; }

std::string SequenceItem::GetName() const
{ return name_; }

const std::optional<png::image<png::rgba_pixel>>& SequenceItem::GetCellMap()
{ return images_view_.GetCellMap(); }

SpriteAnimationCreateInfo SequenceItem::GetAnimationCreateInfo()
{
    std::vector<AnimationPattern> animation_pattern(image_paths_.size());
    for (auto it = image_paths_.begin();it != image_paths_.end();++it)
    {
        size_t d = std::distance(image_paths_.begin(), it);
        double wait = 0;
        if (auto next_it = std::next(it); next_it != image_paths_.end()) {
            wait = next_it->first - it->first;
        } else {
            wait = (end_frame_.value_or(it->first) + 1) - it->first;
        }
        animation_pattern[d] = AnimationPattern{wait,d,0};
    }
    return {name_,
        0,
        0,
        static_cast<double>(images_view_.MaxCellSize().x),
        static_cast<double>(images_view_.MaxCellSize().y),
        is_loop_,
        animation_pattern};
}
