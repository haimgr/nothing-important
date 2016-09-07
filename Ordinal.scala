package game.entities

import scala.collection.mutable
import scala.collection.mutable.ArrayBuffer

/**
  * Created by haim7 on 07 ספטמבר 2016.
  */
class Ordinal[T <: Ordinal[T]](creator: OrdinalCreator[T]) extends Ordered[T] {

  private val self = this.asInstanceOf[T]

  val id: Int = creator.add(self)

  def +(that: T): Set[T] = Set(self, that)

  override def toString: String = getClass.getSimpleName + "(" + id + ")"

  override def compare(that: T): Int = creator.compare(self, that)
}

trait OrdinalCreator[T <: Ordinal[T]] extends Ordering[T] {

  private var _values = new ArrayBuffer[T]

  def add(ordinal: T): Int = {
    val ret = _values.size
    _values += ordinal
    ret
  }

  lazy val values: Seq[T] = {val ret = _values.toList; _values = null; ret}

  override def compare(x: T, y: T): Int = x.id - y.id

}