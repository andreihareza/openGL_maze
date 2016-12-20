#include "utility.hpp"
#include "CDropItem.hpp"
#include "CDropEffectPositive.hpp"
#include "CDropEffectNegative.hpp"

CDropItem::CDropItem(CDropItem::Type type, IDropEffectListener & listener)
: mLastTimeUsed(0ms)
, mType(type)
, mListener(listener)
{
}

bool CDropItem::canAppear()
{
    return (mEffect == nullptr)
        && (NUtility::getCurrentTime() - mLastTimeUsed > NUtility::effectCooldown);
}

bool CDropItem::exists()
{
    return mEffect != nullptr;
}

NUtility::Position CDropItem::appear(
        NUtility::Position playerPosition,
        const std::vector<NUtility::Position> & path,
        std::uint32_t up, std::uint32_t down,
        std::uint32_t left, std::uint32_t right)
{
    std::cout << "CDropItem::" << __func__ << "(): " <<
        (mType == Type::Positive? "Positive" : "Negative") << std::endl;
    switch (mType)
    {
        case Type::Positive:
            mEffect = CDropEffectPositive::randomEffect(mListener);
            break;
        case Type::Negative:
            mEffect = CDropEffectNegative::randomEffect(mListener);
            break;
        default:
            throw 0;
            break;
    }

    if (mEffect != nullptr)
    {
        mPosition = mEffect->getSpawnPosition(playerPosition, path,
                up, down, left, right);
    }
    else
    {
        std::cout << "CDropItem::" << __func__ << "(): " << "getSpawnPosition failed" << std::endl;
    }

    std::cout << "CDropItem::" << __func__ << "(): " << mPosition.first << ' ' << mPosition.second << std::endl;

    return mPosition;
}

void CDropItem::pick()
{
    if (mEffect != nullptr)
    {
        mEffect->picked();

        mEffect = nullptr;
        mPosition = {0, 0};

        mLastTimeUsed = NUtility::getCurrentTime();
    }
}

NUtility::Position CDropItem::getPosition()
{
    return mPosition;
}

glm::vec4 CDropItem::getColor()
{
    if (mEffect != nullptr)
    {
        return mEffect->getColor();
    }
    else
    {
        return {1.0f, 1.0f, 1.0f, 1.0f};
    }
}

