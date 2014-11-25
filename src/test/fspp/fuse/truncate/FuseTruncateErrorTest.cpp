#include "testutils/FuseTruncateTest.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "fspp/impl/FuseErrnoException.h"

using ::testing::_;
using ::testing::StrEq;
using ::testing::Throw;
using ::testing::WithParamInterface;
using ::testing::Values;

using namespace fspp;

class FuseTruncateErrorTest: public FuseTruncateTest, public WithParamInterface<int> {
};
INSTANTIATE_TEST_CASE_P(FuseTruncateErrorTest, FuseTruncateErrorTest, Values(EACCES, EFAULT, EFBIG, EINTR, EINVAL, EIO, EISDIR, ELOOP, ENAMETOOLONG, ENOENT, ENOTDIR, EPERM, EROFS, ETXTBSY));

TEST_P(FuseTruncateErrorTest, TruncateFile) {
  ReturnIsFileOnLstat(FILENAME);
  EXPECT_CALL(fsimpl, truncate(StrEq(FILENAME), _))
    .Times(1).WillOnce(Throw(FuseErrnoException(GetParam())));

  int fd = TruncateFileAllowError(FILENAME, 0);
  EXPECT_EQ(GetParam(), errno);
  EXPECT_EQ(-1, fd);
}